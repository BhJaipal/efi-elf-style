#include <efi-string.h>
#include <efi-err.h>
#include <efi-lib.h>
#include <efi-prot.h>
#include <efi-ser.h>

#define KERNEL_EXECUTABLE_PATH u"\\kernel.elf"

typedef struct s_boot_video_info {
	void    *framebuffer_pointer;
	uint32  horizontal_resolution;
	uint32  vertical_resolution;
	uint32  pixels_per_scanline;
} Kernel_Boot_Video_Mode_Info;

/**
 * @brief Kernel boot info struct.
 * Contains information passed to the kernel at boot time.
 * This should be kept separate from the kernel implementation to improve the
 * portability of the implementation:
 * This definition uses the `EFI_MEMORY_DESCRIPTOR` and `UINTN` types so that it will
 * automatically use the correct types for the target architecture. The corresponding
 * definition within the kernel should have more architecture-specific types.
 */
typedef struct s_boot_info {
	efi_memory_descriptor_t        *memory_map;
	uint64                         memory_map_size;
	uint64                         memory_map_descriptor_size;
	Kernel_Boot_Video_Mode_Info    video_mode_info;
} Kernel_Boot_Info;

efi_status load_kernel_image(input efi_file_t* const root_file_system,
	input wuchar* kernel_image_filename,
	output efi_virtual_addr_t* kernel_entry_point) {
	efi_file_t *kernel_img = NULL;
	efi_status s = root_file_system->open(root_file_system, &kernel_img, (wchar*)kernel_image_filename, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);

	return s;
}

efi_status efi_main(efi_handle_t img_handle, efi_system_table_t *system_table) {
	system_table->cout->set_attribute(system_table->cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_GREEN));
	system_table->cout->clear_screen (system_table->cout);
	initialize_lib(img_handle, system_table);


	efi_guid_t gEfiSimpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
	uint64 fs_handle_sz = 0;
	efi_handle_t *fs_handles = NULL;

	if (EFI_ERROR(global.boot->locate_handle_buffer(BY_PROTOCOL, &gEfiSimpleFileSystemProtocolGuid, NULL, &fs_handle_sz, &fs_handles))) {
		printf("Can't load fs\r\n");
	} else {
		printf("Loaded fs, %d handles\r\n", fs_handle_sz);
	}

	efi_simple_file_system_protocol_t *fs_proto = NULL;
	if (EFI_ERROR(global.boot->locate_protocol(&gEfiSimpleFileSystemProtocolGuid, NULL, (void**)&fs_proto))) {
		printf("FS protocol error\r\n");
	} else {
		printf("FS protocol loaded\r\n");
	}

	efi_file_t *root_fs = NULL;
	efi_physical_addr_t kernel_entry_point = 0;
	efi_memory_descriptor_t *mmap_desc = NULL;
	void (*kernel_entry)(Kernel_Boot_Info* boot_info);
	Kernel_Boot_Info boot_info = {0};

	if (EFI_ERROR(fs_proto->open_volume(fs_proto, &root_fs))) {
		printf("Can't load rootFS\r\n");
	} else {
		printf("RootFS loaded\r\n");
	}

	if (EFI_ERROR(load_kernel_image(root_fs, KERNEL_EXECUTABLE_PATH, &kernel_entry_point))) {
		printf("Can't find kernel\r\n");
	} else {
		printf("Loaded kernel\r\n");
	}

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

