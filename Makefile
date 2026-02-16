CFLAGS  = -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -mabi=ms
LDFLAGS = -nostdlib -znocombreloc -T /usr/lib/elf_x86_64_efi.lds -shared -Bsymbolic /usr/lib/crt0-efi-x86_64.o

build/%.o: %.c
	gcc -I/usr/include/efi/ -Iinclude $(CFLAGS) -Wall -c $< -o $@


build/main.so: build/example/new.o build/src/efi-string.o
	ld $(LDFLAGS) $^ -o $@ /usr/lib/libgnuefi.a /usr/lib/libefi.a

main.efi: build/main.so
	if [ ! -f main.efi ]; then touch $@; fi
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym  \
    -j .rel -j .rela -j .reloc --target=efi-app-x86_64 $< $@

boot: main.efi
	# 1. Create a 64MB empty file
	dd if=/dev/zero of=uefi.img bs=1M count=64
	
	# 2. Create GPT partition table and a single EFI partition
	parted uefi.img -s mklabel gpt
	parted uefi.img -s mkpart EFI fat32 1MiB 100%
	parted uefi.img -s set 1 esp on
	
	# 3. Format the partition (offset 2048 sectors * 512 bytes = 1048576)
	# We use the @@ offset syntax for mtools/mkfs
	mkfs.vfat --offset 2048 uefi.img
	
	# 4. Copy the file using the offset
	mmd -i uefi.img@@1M ::/EFI
	mmd -i uefi.img@@1M ::/EFI/BOOT
	echo "FS0:\EFI\BOOT\BOOTX64.EFI" > startup.nsh
	
	# Copy the script to the root of the image
	mcopy -i uefi.img@@1M startup.nsh ::/startup.nsh
	
	# Copy your EFI binary as usual
	mcopy -i uefi.img@@1M $< ::/EFI/BOOT/BOOTX64.EFI
	
	# Run QEMU (adding -show-cursor can help if you're doing GUI stuff)
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive file=uefi.img,format=raw -net none
