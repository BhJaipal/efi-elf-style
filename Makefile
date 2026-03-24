CFLAGS  = -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -mabi=ms -Wall -Wno-pointer-to-int-cast
LDFLAGS = -nostdlib \
    		-Wl,-subsystem,10 \
    		-Wl,-entry,efi_main \
			-Wl,--pic-executable \
    		-Wl,--file-alignment,512 \
    		-Wl,--section-alignment,4096

ARCH = x86_64-w64-mingw32
CC = $(ARCH)-gcc-win32

define SRC_to_OBJ
build/$(basename $(1)).o
endef

RED          := \033[31m
BLUE         := \033[34m
CYAN         := \033[36m
GREEN        := \033[32m
YELLOW       := \033[33m
BOLD         := \033[1m
NC           := \033[0m

PRINT_STEP_DEL =   @printf "  $(RED)%-7s$(NC)  $(BOLD)%s$(NC)\n" "$(1)" "$(2)"
PRINT_STEP =       @printf "  $(CYAN)%-7s$(NC)  $(BOLD)%s$(NC)\n" "$(1)" "$(2)"
PRINT_STEP_MSDOS = @printf "  $(YELLOW)%-7s$(NC)  $(BOLD)%s$(NC)\n" "$(1)" "$(2)"

SRC := $(wildcard src/*.c) src/efi_stub.S
OBJ := $(foreach src, $(SRC), $(call SRC_to_OBJ,$(src)))

MAIN = example/new.c

ELF_OBJ = $(foreach src, $(wildcard example/kernel/*), $(subst example,build,$(basename $(src)).o))

all: run

build/%.o: %.S
	$(call PRINT_STEP, "CC", $(<:.S=.o))
	@$(CC) -Iinclude $(CFLAGS) -Wall -c $< -o $@

build/%.o: %.c
	$(call PRINT_STEP, "CC", $(<:.c=.o))
	@$(CC) -Iinclude $(CFLAGS) -Wall -c $< -o $@


main.efi: $(call SRC_to_OBJ,$(MAIN)) $(OBJ)
	$(call PRINT_STEP, "CC", $@)
	@$(CC) $(LDFLAGS) -o $@ $^
	@$(ARCH)-objcopy -R .comment -R .note -R .note.gnu.build-id main.efi


build/kernel/%.o: example/kernel/%.asm
	$(call PRINT_STEP, "AS", $(<:.c=.o))
	@as $< -o $@

build/kernel/%.o: example/kernel/%.c
	$(call PRINT_STEP, "CC", $(<:.c=.o))
	@gcc $< -c -o $@ -ffreestanding -fno-pie -fPIE -Iinclude/kernel

kernel.elf: build/kernel/kernel.o build/kernel/entry.o build/kernel/port.o build/kernel/uart.o build/kernel/vga.o
	$(call PRINT_STEP, "LD", $@)
	@ld -T./example/kernel/kernel.ld -o $@ --entry kernel_entry $^


uefi.img: main.efi kernel.elf
	@printf "\e[1;32m CREATING DISK IMG\e[0m\n"
	@# 1. Create a 64MB empty file
	$(call PRINT_STEP_MSDOS, "DD", $@)
	@dd if=/dev/zero of=$@ bs=1M count=64 2> /dev/null
	
	@# 2. Create GPT partition table and a single EFI partition
	$(call PRINT_STEP_MSDOS, "PARTED", $@)
	@parted $@ -s mklabel gpt
	@parted $@ -s mkpart EFI fat32 1MiB 100%
	@parted $@ -s set 1 esp on
	
	$(call PRINT_STEP_MSDOS, "MSDOS", $@)
	@mformat -i $@@@1M -F
	@mmd     -i $@@@1M ::/EFI
	@mmd     -i $@@@1M ::/EFI/BOOT
	@mcopy   -i $@@@1M $< ::/EFI/BOOT/BOOTX64.EFI
	@mcopy   -i $@@@1M kernel.elf ::/kernel.elf

	
run: uefi.img
	@printf "  $(GREEN) QEMU $(NC)  $(BOLD)  $<$(NC)\n"
	@qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive file=$<,format=raw -net none

.ONESHELL:
clean: $(OBJ) $(ELF_OBJ) build/example/new.o main.efi uefi.img kernel.elf
	@for file in $^; do
		printf "  $(RED)%-7s$(NC)  $(BOLD)%s$(NC)\n" "RM" "$$file"; \
		rm $$file; \
	done
