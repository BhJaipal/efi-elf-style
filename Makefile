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

SRC := $(wildcard src/*.c) src/efi_stub.S
OBJ := $(foreach src, $(SRC), $(call SRC_to_OBJ,$(src)))

MAIN = example/new.c

all: run

build/%.o: %.S
	$(CC) -I/usr/include/efi/ -Iinclude $(CFLAGS) -Wall -c $< -o $@

build/%.o: %.c
	$(CC) -I/usr/include/efi/ -Iinclude $(CFLAGS) -Wall -c $< -o $@


main.efi: $(call SRC_to_OBJ,$(MAIN)) $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^
	$(ARCH)-objcopy -R .comment -R .note -R .note.gnu.build-id main.efi


uefi.img: main.efi
	# 1. Create a 64MB empty file
	dd if=/dev/zero of=$@ bs=1M count=64
	
	# 2. Create GPT partition table and a single EFI partition
	parted $@ -s mklabel gpt
	parted $@ -s mkpart EFI fat32 1MiB 100%
	parted $@ -s set 1 esp on
	
	mformat -i $@@@1M -F
	mmd     -i $@@@1M ::/EFI
	mmd     -i $@@@1M ::/EFI/BOOT
	mcopy   -i $@@@1M $< ::/EFI/BOOT/BOOTX64.EFI
	
run: uefi.img
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive file=$<,format=raw -net none
