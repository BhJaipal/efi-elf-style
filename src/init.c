#include "efi-api.h"
#include <efi-prot.h>
#include "types.h"
#include <efi-lib.h>
#include <efi-err.h>

Efi_lib_flags_t efi_flags = {0};
global_t global = {0};

void initialize_lib(efi_handle_t ImageHandle, efi_system_table_t *SystemTable) {
	if (efi_flags.lib_initialized) return;

    global.sys = SystemTable;
    global.boot = SystemTable->boot_services;
    global.runtime = SystemTable->runtime_services;

	if (ImageHandle) {
		efi_debug_variable();
	}
}
// NOTE: put it in efi-debug.h
uint64     efi_debug;

efi_guid_t efi_global_variable;
void efi_debug_variable() {
	efi_status status;
	uint32 attributes;
	uint64 data_size;
	uint64 new_efi_debug;

	data_size = sizeof(efi_debug);
	status = global.runtime->variable.get_variable((short*)u"EFIDebug", &efi_global_variable, &attributes, &data_size, &new_efi_debug);
	if (EFI_ERROR(status)) {
		efi_debug = new_efi_debug;
	}
}


void shutdown() {
	global.runtime->reset_system(EFI_RESET_SHUTDOWN, 0, 0, NULL);
	__builtin_unreachable();
}

void exit(efi_status exit_status, int64 data_size, int16 *data) {
	global.runtime->reset_system(EFI_RESET_SHUTDOWN, exit_status, data_size, data);
	__builtin_unreachable();
}

efi_status find_video_mode(input efi_graphics_output_protocol_t* const protocol,
	input const uint32 target_width,
	input const uint32 target_height,
	input const efi_graphics_pixel_format_t target_pixel_format,
	output uint64* video_mode)
{
	/** Program status. */
	efi_status status;
	/** The size of the video mode info struct. */
	uint64 size_of_mode_info;
	/** The video mode info struct. */
	efi_graphics_output_mode_information_t* mode_info;

	uint64 i = 0;
	for(i = 0; i < protocol->mode->max_mode; i++) {

		status = protocol->query_mode(
			protocol, i, &size_of_mode_info, &mode_info);
		if(EFI_ERROR(status)) {
			return status;
		}

		if(mode_info->horizontal_resolution == target_width &&
			mode_info->vertical_resolution == target_height &&
			mode_info->pixel_format == target_pixel_format) {
			*video_mode = i;
			return EFI_SUCCESS;
		}
	}

	return EFI_UNSUPPORTED;
}
efi_status set_graphics_mode(input efi_graphics_output_protocol_t* const protocol,
	input const uint32 target_width,
	input const uint32 target_height,
	input const efi_graphics_pixel_format_t target_pixel_format) {
	efi_status status = 0;
	uint64 graphics_mode_num = 0;

	status = find_video_mode(protocol, target_width, target_height,
		target_pixel_format, &graphics_mode_num);
	if(EFI_ERROR(status)) {
		return status;
	}

	protocol->set_mode(protocol, graphics_mode_num);
	return status;
}
void init_graphics(efi_handle_t img_handle, Kernel_Boot_Video_Mode_Info *boot_info, efi_handle_t **handle_buffer) {
	efi_graphics_output_protocol_t *graphics_out_proto = NULL;
	efi_guid_t gra_out_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	uint64 handle_count;
	efi_status s = global.boot->locate_handle_buffer(BY_PROTOCOL, &gra_out_guid, NULL, &handle_count, handle_buffer);
	ON_ERR(s,, "can't load GO proto\r\n");
	s = global.boot->open_protocol(global.sys->console_out_handle, &gra_out_guid, (void**)&graphics_out_proto, img_handle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
	ON_ERR(s,, "can't connect GO proto to cout handle\r\n");
	if (graphics_out_proto) {
		s = set_graphics_mode(graphics_out_proto, 640 * 3, 480 * 3, PIXEL_BGR_RESERVED_8BIT_PER_COLOR);

		ON_ERR(s,, "can't set graphics mode\r\n");
		boot_info->framebuffer_pointer =
			(void*)graphics_out_proto->mode->frame_buffer_base;
		boot_info->horizontal_resolution =
			graphics_out_proto->mode->info->horizontal_resolution;
		boot_info->vertical_resolution =
			graphics_out_proto->mode->info->vertical_resolution;
		boot_info->pixels_per_scanline =
			graphics_out_proto->mode->info->pixels_per_scan_line;
	}
}
