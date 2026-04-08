CFLAGS  = -ffreestanding -fshort-wchar -mno-red-zone -mabi=ms -Wall -Wextra
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
GREEN        := \033[32m
YELLOW       := \033[33m
BLUE         := \033[94m
PURPLE       := \033[95m
CYAN         := \033[96m
BOLD         := \033[1m
NC           := \033[0m

PRINT_STEP_DEL   = @printf "  $(RED)%-7s$(NC)  $(BOLD)%s$(NC)\n" "$(1)" "$(2)"
PRINT_STEP       = @printf "  $(YELLOW)%-7s$(NC)  $(BOLD)%s$(NC)\n" "$(1)" "$(2)"
PRINT_STEP_CC    = @printf "  $(BLUE)%-7s$(NC)  $(BOLD)%s$(NC)\n" "$(1)" "$(2)"
PRINT_STEP_MSDOS = @printf "  $(CYAN)%-7s$(NC)  $(BOLD)%s$(NC)\n" "$(1)" "$(2)"

SRC := $(wildcard src/*.c) src/efi_stub.S
OBJ := $(foreach src, $(SRC), $(call SRC_to_OBJ,$(src)))

MAIN = example/new.c

ELF_OBJ = $(foreach src, $(wildcard example/kernel/*), $(subst example,build,$(basename $(src)).o))

all: help

dir:
	mkdir -p build/{src,kernel,example,iso}

build/%.o: %.S
	$(call PRINT_STEP_CC, "CC", $(<:.S=.o))
	@$(CC) -Iinclude $(CFLAGS) -Wall -c $< -o $@

build/%.o: %.c
	$(call PRINT_STEP_CC, "CC", $(<:.c=.o))
	@$(CC) -Iinclude $(CFLAGS) -Wall -c $< -o $@

build/kernel/%.o: example/kernel/%.asm
	$(call PRINT_STEP, "AS", $(<:.c=.o))
	@as $< -o $@

build/kernel/%.o: example/kernel/%.c
	$(call PRINT_STEP_CC, "CC", $(<:.c=.o))
	@gcc $< -c -o $@ -ffreestanding -fno-pie -fPIE -Iinclude/kernel

build/BOOTX64.img: $(call SRC_to_OBJ,$(MAIN)) $(OBJ)
	$(call PRINT_STEP_CC, "CC", $@)
	@$(CC) $(LDFLAGS) -o $@ $^
	@$(ARCH)-objcopy -R .comment -R .note -R .note.gnu.build-id $@

build/kernel.img: build/kernel/entry.o build/kernel/kernel.o build/kernel/port.o build/kernel/uart.o build/kernel/vga.o
	$(call PRINT_STEP, "LD", $@)
	@ld -T./example/kernel/kernel.ld -o $@ --entry kernel_entry $^


build/uefi.img: build/BOOTX64.img build/kernel.img
	@printf "\e[1;32m CREATING DISK IMG\e[0m\n"
	@# 1. Create a 64MB empty file
	$(call PRINT_STEP_MSDOS, "DD", $@)
	@if [ -f $@ ]; then rm $@; fi
	@dd if=/dev/zero of=$@ bs=16M count=4 2> /dev/null
	
	@# 2. Create GPT partition table and a single EFI partition
	$(call PRINT_STEP_MSDOS, "PARTED", $@)
	@parted $@ -s mklabel gpt
	@parted $@ -s mkpart EFI fat32 2MiB 100%
	@parted $@ -s set 1 esp on
	
	$(call PRINT_STEP_MSDOS, "MSUTIL", $@)
	@mformat -i $@@@2M    -F -n "NerOS"
	@mmd     -i $@@@2M    ::/EFI
	@mmd     -i $@@@2M    ::/EFI/BOOT
	@mcopy   -i $@@@2M $< ::/EFI/BOOT/BOOTX64.EFI
	@mcopy   -i $@@@2M build/kernel.img ::/kernel.elf

build/iso/efiboot.img: build/BOOTX64.img build/kernel.img
	$(call PRINT_STEP_MSDOS, "DD", $@)
	@echo "" > $@
	@dd if=/dev/zero of=$@ bs=1M count=17 2> /dev/null
	
	$(call PRINT_STEP_MSDOS, "MSUTIL", $@)
	@mkfs.vfat $@ -F 16  > /dev/null
	@mmd   -i  $@                   ::/EFI
	@mmd   -i  $@                   ::/EFI/BOOT
	@mcopy -i  $@ build/BOOTX64.img ::/EFI/BOOT/BOOTX64.EFI
	@mcopy -i  $@ build/kernel.img  ::/kernel.elf
	@mkdir -p  build/iso/EFI/BOOT

build/uefi.iso: build/iso/efiboot.img
	@printf "   $(CYAN)%-7s$(NC)  $(BOLD)%s$(NC)\n" "XORRISO" "$@"
	@xorriso -as mkisofs \
		-R -J -V "NerOS" \
		-e $$(basename $<) \
		-no-emul-boot \
		-append_partition 2 0xef $< \
		-isohybrid-gpt-basdat \
		-o $@ build/iso/ 2> /dev/null

img: build/uefi.img
	@printf "  $(GREEN) QEMU $(NC)  $(BOLD)  $<$(NC)\n"
	@qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive file=$<,format=raw -net none
	@printf "\n\r"

iso: build/uefi.iso
	@printf "  $(GREEN) QEMU $(NC)  $(BOLD)  $<$(NC)\n"
	@qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -cdrom $< -m 256M
	@printf "\n\r"

.ONESHELL:
clean: $(OBJ) $(ELF_OBJ) build/example/new.o build/BOOTX64.img build/uefi.img build/kernel.img
	@for file in $^; do
		printf "  $(RED)%-7s$(NC)  $(BOLD)%s$(NC)\n" "RM" "$$file"; \
		rm $$file; \
	done

help:
	@printf "$(YELLOW)help               $(GREEN)Shows this help                        $(NC)\n"
	@printf "$(YELLOW)dir                $(GREEN)Creates essential dirs, Use it 1st     $(NC)\n"
	@printf "$(YELLOW)clean              $(GREEN)Removes all build files                $(NC)\n"
	@printf "$(YELLOW)build/efiboot.img  $(GREEN)Creates example OS ISO image sector    $(NC)\n"
	@printf "$(YELLOW)build/uefi.img     $(GREEN)Creates example OS image               $(NC)\n"
	@printf "$(YELLOW)build/uefi.iso     $(GREEN)Creates example OS ISO                 $(NC)\n"
	@printf "$(YELLOW)build/kernel.img   $(GREEN)Compiles example kernel image          $(NC)\n"
	@printf "$(YELLOW)build/BOOTX64.img  $(GREEN)Compiles example bootloader image      $(NC)\n"
	@printf "$(YELLOW)img                $(GREEN)Run OS image                           $(NC)\n"
	@printf "$(YELLOW)iso                $(GREEN)Run OS ISO                             $(NC)\n"

