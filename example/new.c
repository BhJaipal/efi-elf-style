#include "efi-api.h"
#include "types.h"
#include <efi-err.h>
#include <efi-lib.h>
#include <efi-prot.h>
#include <efi-ser.h>
#include <efi-elf.h>
#include <efi-asm.h>

#define KERNEL_EXECUTABLE_PATH u"\\kernel.elf"

typedef struct {
	efi_memory_descriptor_t        *memory_map;
	uint64                         memory_map_size;
	uint64                         memory_map_descriptor_size;
	Kernel_Boot_Video_Mode_Info    video_mode_info;
} Kernel_Boot_Info;

efi_status efi_main(efi_handle_t img_handle, efi_system_table_t *system_table) {
	system_table->cout->set_attribute(system_table->cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_GREEN));
	system_table->cout->clear_screen (system_table->cout);
	initialize_lib(img_handle, system_table);

	efi_handle_t *graphics_handle_buffer;

	/** Function pointer to the kernel entry point. */
	void (*kernel_entry)(Kernel_Boot_Info*);

	efi_memory_descriptor_t *mmap = NULL;
	uint64 memory_map_key = 0;
	/** The size of the memory map buffer. */
	uint64 memory_map_size = 0;
	/** The memory map descriptor size. */
	uint64 descriptor_size;
	/** The memory map descriptor. */
	uint32 descriptor_version;

	efi_status s = global.boot->memory.get_memory_map(&memory_map_size, NULL, &memory_map_key, &descriptor_size, NULL);

	if (s < 0) {
        // Add a safety margin of at least 4 descriptors. 
        // Allocating memory can actually create new map entries!
        memory_map_size += (descriptor_size * 4);
        
        s = global.boot->memory.allocate_pool(EFI_LOADER_DATA, memory_map_size, (void**)&mmap);
        ON_ERR(s, return s, "Failed to allocate memory map pool\r\n");
    } else {
        // If it didn't return 'too small', something else is wrong
        ON_ERR(s, return s, "Unexpected MMap probe status\r\n");
    }
	printf("Back out\r\n");

	efi_guid_t gSerial = EFI_SERIAL_IO_PROTOCOL_GUID;
	efi_serial_io_protocol_t *serial_proto = NULL;
	s = global.boot->locate_protocol(&gSerial, NULL, (void**)&serial_proto);
	ON_ERR(s, , "Can't get serial protocol\r\n");
	s = serial_proto->set_attributes(serial_proto, 0, 0, 0, 0, 0, DEFAULT_STOP_BITS);
	ON_ERR(s, , "Can't set serial attr\r\n");


	Kernel_Boot_Info boot_info = {0};
	init_graphics(img_handle, &boot_info.video_mode_info, &graphics_handle_buffer);

	efi_guid_t gEfiSimpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
	uint64 fs_handle_sz = 0;
	efi_handle_t *fs_handles = NULL;

	if (EFI_ERROR(global.boot->locate_handle_buffer(BY_PROTOCOL, &gEfiSimpleFileSystemProtocolGuid, NULL, &fs_handle_sz, &fs_handles))) {
		printf("Can't load FS handle\r\n");
	} else {
		printf("Loaded FS handle, %d handles\r\n", fs_handle_sz);
	}

	efi_simple_file_system_protocol_t *fs_proto = NULL;
	if (EFI_ERROR(global.boot->locate_protocol(&gEfiSimpleFileSystemProtocolGuid, NULL, (void**)&fs_proto))) {
		printf("FS protocol error\r\n");
	} else {
		printf("FS protocol loaded\r\n");
	}

	efi_file_t *root_fs = NULL;

	if (EFI_ERROR(fs_proto->open_volume(fs_proto, &root_fs))) {
		printf("Can't load RootFS\r\n");
	} else {
		printf("RootFS loaded\r\n");
	}

	Elf64 *kernel = NULL;
	global.boot->memory.allocate_pool(EFI_LOADER_DATA, sizeof(Elf64), (void**)&kernel);
	load_kernel_image(root_fs, KERNEL_EXECUTABLE_PATH, kernel);

	global.boot->memory.free_pool(graphics_handle_buffer);

	kernel_entry = (void (*)(Kernel_Boot_Info*))kernel->head.e_entry;
	printf("Run kernel from here\r\n");


	s = global.boot->memory.get_memory_map(&memory_map_size, mmap, &memory_map_key,
		&descriptor_size, &descriptor_version);

	ON_ERR(s,, "Can't get MMap\r\n");

	boot_info.memory_map = mmap;
	boot_info.memory_map_size = memory_map_size;
	boot_info.memory_map_descriptor_size = descriptor_size;

	// Your GDT entries
	__attribute__((aligned(16)))
	uint64 gdt64[] = {
		0x0000000000000000, // Null
		0x00209a0000000000, // Code (Offset 0x08)
		0x0000920000000000  // Data (Offset 0x10)
	};

	struct GDTDescriptor gdt_ptr = {
		.size = sizeof(gdt64) - 1,
		.offset = (uint64)gdt64
	};

	s = global.boot->image.exit_boot_services(img_handle, memory_map_key);
	ON_ERR(s,, "Can't exit boot service %p %d\r\n", img_handle, memory_map_key);

	uint64 boot_info_addr = (uint64)&boot_info;
	asm_cli();
	asm_lgdt(gdt_ptr);
	asm volatile(
        "pushq $0x10\n\t"            // Push Data Segment selector (0x10)
        "pushq %%rsp\n\t"            // Push current Stack Pointer
        "pushfq\n\t"                 // Push Flags
        "pushq $0x08\n\t"            // Push Code Segment selector (0x08)
        "pushq %[entry]\n\t"         // Push the address to jump to
        "movq %[info], %%rdi\n\t"    // Pass boot info pointer to kernel (System V ABI)
        "iretq"                      // The "Magic" jump
        :
        : [entry] "m"(kernel_entry), [info] "m"(boot_info_addr)
        : "memory"
    );

	printf("Back to bootloader\r\n");

	efi_input_key_t key;
	printf("\nPress:\r\n    [Qq]: exit\r\n    [Kk]: boot menu\r\n");
	while (true) {
		if (!system_table->cin->read_key_stroke(system_table->cin, &key)) {
			printf("Current input: %c\r", key.unicode_char);
			if (key.unicode_char == 'Q' || key.unicode_char == 'q') {
				shutdown();
			} else if (key.unicode_char == 'k' || key.unicode_char == 'K') {
				break;
			}
		}
		asm("hlt");
	}
	return EFI_SUCCESS;
}
