#include "efi-api.h"
#include "efi-bind.h"
#include "efi-con.h"
#include "types.h"
#include <efi-string.h>
#include <efi-err.h>
#include <efi-lib.h>
#include <efi-prot.h>
#include <efi-ser.h>

#define KERNEL_EXECUTABLE_PATH L"\\kernel.elf"

typedef struct s_boot_video_info {
	void    *framebuffer_pointer;
	uint32  horizontal_resolution;
	uint32  vertical_resolution;
	uint32  pixels_per_scanline;
} Kernel_Boot_Video_Mode_Info;

efi_guid_t gEfiSerialIoProtocolGuid = EFI_SERIAL_IO_PROTOCOL_GUID;
efi_guid_t gEfiGraphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
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

typedef struct s_uefi_graphics_service {
	efi_handle_t* handle_buffer;
	uint64 handle_count;
} Uefi_Graphics_Service;
void init_Serial_service(efi_serial_io_protocol_t **serial);
void init_graphics_out_Service(Uefi_Graphics_Service *graphics_service);

__attribute__((ms_abi))
efi_status efi_main(efi_handle_t img_handle, efi_system_table_t *system_table) {
	system_table->cout->set_attribute(system_table->cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_GREEN));
	system_table->cout->clear_screen (system_table->cout);
	initialize_lib(img_handle, system_table);
	long x = printf("Hello %s %d %f %d\r\n", "world", 73, -2.34, 0.0);
	printf("Bytes written to screen: %d\r\nPress:\r\n    Qq to exit\r\n    Kk to open boot menu\r\n", x);

	efi_input_key_t key;
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

	Uefi_Graphics_Service graphics_out_proto;
	efi_file_t *root_fs = NULL;
	efi_simple_file_system_protocol_t *fs_service = NULL;
	efi_serial_io_protocol_t *serial_service = NULL;

	efi_physical_addr_t kernel_entry_point = 0;
	efi_memory_descriptor_t *mmap_desc = NULL;
	void (*kernel_entry)(Kernel_Boot_Info* boot_info);
	Kernel_Boot_Info boot_info = {0};

	uefi_call_wrapper(global.boot->set_watchdog_timer, 4, 0, 0, 0, NULL);
	uefi_call_wrapper(global.sys->cin->reset, 2, global.sys->cin, 0);
	init_Serial_service(&serial_service);
	init_graphics_out_Service(&graphics_out_proto);

	return EFI_SUCCESS;
}

efi_status configure_serial_protocol(input efi_serial_io_protocol_t* const protocol) {
	return uefi_call_wrapper(protocol->set_attributes, 7,
		protocol, 0, 0, 0, 0, 0, DEFAULT_STOP_BITS);
}

void init_Serial_service(efi_serial_io_protocol_t **serial) {
	if (uefi_call_wrapper(global.boot->locate_protocol, 3, &gEfiSerialIoProtocolGuid, NULL, serial)) {
		printf("Init serial service Error\r\n");
	}
	if (configure_serial_protocol(*serial)) {
		printf("Config serial service Error\r\n");
	}
}

void init_graphics_out_Service(Uefi_Graphics_Service *graphics_service) {
	if (uefi_call_wrapper(global.boot->locate_handle_buffer, 5,
		BY_PROTOCOL, &gEfiGraphicsOutputProtocolGuid, NULL,
		&graphics_service->handle_count, &graphics_service->handle_buffer)) {
		printf("Graphics service Error\r\n");
	}
}
