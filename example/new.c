#include "efi-api.h"
#include "types.h"
#include <efi-err.h>
#include <efi-lib.h>
#include <efi-prot.h>
#include <efi-ser.h>
#include <efi-elf.h>

#define KERNEL_EXECUTABLE_PATH u"\\kernel.elf"

#define ON_ERR(status, body, ...)       \
	if (status < 0) {                   \
		efi_error(status, __VA_ARGS__); \
		body;                           \
	}

typedef struct {
	void    *framebuffer_pointer;
	uint32  horizontal_resolution;
	uint32  vertical_resolution;
	uint32  pixels_per_scanline;
} Kernel_Boot_Video_Mode_Info;

void *memcpy(void *dest, const void *src, uint64 n) {
	for (uint64 i = 0; i < n; i++) {
		((char*)dest)[i] = ((char*)src)[i];
	}
	return dest;
}

int memcmp(const void *dest, const void *src, uint64 n) {
	for (uint64 i = 0; i < n; i++) {
		if (((char*)dest)[i] != ((char*)src)[i])
			return 1;
	}
	return 0;
}

/**
 * @brief Kernel boot info struct.
 * Contains information passed to the kernel at boot time.
 * This should be kept separate from the kernel implementation to improve the
 * portability of the implementation:
 * This definition uses the `efi_memory_descriptor_t` and `uint64` types so that it will
 * automatically use the correct types for the target architecture. The corresponding
 * definition within the kernel should have more architecture-specific types.
 */
typedef struct {
	efi_memory_descriptor_t        *memory_map;
	uint64                         memory_map_size;
	uint64                         memory_map_descriptor_size;
	Kernel_Boot_Video_Mode_Info    video_mode_info;
} Kernel_Boot_Info;

void load_kernel_image(
	input efi_file_t* root_file_system,
	input wuchar* kernel_image_filename,
	Elf64 *elf_object
);

void init_graphics(efi_handle_t img_handle, Kernel_Boot_Info *boot_info, efi_handle_t **handle_buffer);

efi_status efi_main(efi_handle_t img_handle, efi_system_table_t *system_table) {
	system_table->cout->set_attribute(system_table->cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_GREEN));
	system_table->cout->clear_screen (system_table->cout);
	initialize_lib(img_handle, system_table);

	efi_handle_t *graphics_handle_buffer;

	/** Function pointer to the kernel entry point. */
	void (*kernel_entry)(Kernel_Boot_Info*);
	/** Boot info struct, passed to the kernel. */
	Kernel_Boot_Info boot_info = {0};

	efi_memory_descriptor_t *mmap = NULL;
	uint64 memory_map_key = 0;
	/** The size of the memory map buffer. */
	uint64 memory_map_size = 0;
	/** The memory map descriptor size. */
	uint64 descriptor_size;
	/** The memory map descriptor. */
	uint32 descriptor_version;

	efi_status s = global.boot->memory.get_memory_map(&memory_map_size, NULL, &memory_map_key, &descriptor_size, NULL);
	ON_ERR(s, , "Can't get MMap by test\r\n");

	memory_map_size += (2 * (descriptor_size));
	s = global.boot->memory.allocate_pool(EFI_LOADER_DATA, memory_map_size, (void**)&mmap);
	ON_ERR(s, return s, "Can't allocate MMap pool\r\n");

	efi_guid_t gSerial = EFI_SERIAL_IO_PROTOCOL_GUID;
	efi_serial_io_protocol_t *serial_proto = NULL;
	global.boot->locate_protocol(&gSerial, NULL, (void**)&serial_proto);
	serial_proto->set_attributes(serial_proto, 0, 0, 0, 0, 0, DEFAULT_STOP_BITS);
	init_graphics(img_handle, &boot_info, &graphics_handle_buffer);

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

	s = global.boot->image.exit_boot_services(img_handle, memory_map_key);
	ON_ERR(s,, "Can't exit boot service %p %d\r\n", img_handle, memory_map_key);
	
	char c = 0;
	uint32 colour = 0;
	for (int i = 0;  i < 60; i++) {
		for (int j = 0; j < 60; j++) {
			c = (i ^ j) % 256;
			colour = ((255 - (c % 128)) << 16) | (c << 8) | (c % 128);
			uint32* at = boot_info.video_mode_info.framebuffer_pointer + i + (j * boot_info.video_mode_info.pixels_per_scanline);
			*at = colour;
		}
	}
	kernel_entry(&boot_info);
	
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

void load_segment(efi_file_t *kernel_img, Elf64_phdr_t *phdr) {
	void *program_data = NULL;
	uint64 buffer_size = 0;
	uint64 page_segment_count = EFI_SIZE_TO_PAGES(phdr->p_memsz);

	efi_status status;


	status = kernel_img->set_position(kernel_img, phdr->p_offset);
	ON_ERR(status,, "  Can't set position\r\n");
	status = global.boot->memory.allocate_pages(ALLOCATE_ADDRESS, EFI_LOADER_DATA, page_segment_count, &phdr->p_phy_addr);
	ON_ERR(status,, "  Can't create memory page\r\n");

	if (phdr->p_filesz) {
		buffer_size = phdr->p_filesz;
		status = global.boot->memory.allocate_pool(EFI_LOADER_CODE, buffer_size, &program_data);
		ON_ERR(status,, "  Can't make memory pool\r\n");

		status = kernel_img->read(kernel_img, &buffer_size, program_data);
		ON_ERR(status,, "  Can't read phdr data\r\n");
		status = global.boot->copy_mem((void*)phdr->p_phy_addr, program_data, phdr->p_filesz);
		ON_ERR(status,, "  Can't copy phdr to physical address\r\n");

		status = global.boot->memory.free_pool(program_data);
		ON_ERR(status,, "  Can't free memory\r\n");
	}

	uint64 zero_fill_count = phdr->p_memsz - phdr->p_filesz;
	efi_physical_addr_t zero_fill_start = phdr->p_phy_addr - phdr->p_filesz;

	if (zero_fill_count) {
		global.boot->set_mem((void*)zero_fill_start, zero_fill_count, 0);
	}
}
void init_graphics(efi_handle_t img_handle, Kernel_Boot_Info *boot_info, efi_handle_t **handle_buffer) {
	efi_graphics_output_protocol_t *graphics_out_proto = NULL;
	efi_guid_t gra_out_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	uint64 handle_count;
	efi_status s = global.boot->locate_handle_buffer(BY_PROTOCOL, &gra_out_guid, NULL, &handle_count, handle_buffer);
	ON_ERR(s,, "can't load GO proto\r\n");
	s = global.boot->open_protocol(global.sys->console_out_handle, &gra_out_guid, (void**)&graphics_out_proto, img_handle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
	ON_ERR(s,, "can't connect GO proto to cout handle\r\n");
	if (graphics_out_proto) {
		s = set_graphics_mode(graphics_out_proto, 1024, 768, PIXEL_BGR_RESERVED_8BIT_PER_COLOR);

		ON_ERR(s,, "can't set graphics mode\r\n");
		boot_info->video_mode_info.framebuffer_pointer =
			(void*)graphics_out_proto->mode->frame_buffer_base;
		boot_info->video_mode_info.horizontal_resolution =
			graphics_out_proto->mode->info->horizontal_resolution;
		boot_info->video_mode_info.vertical_resolution =
			graphics_out_proto->mode->info->vertical_resolution;
		boot_info->video_mode_info.pixels_per_scanline =
			graphics_out_proto->mode->info->pixels_per_scan_line;
	}
}
void load_kernel_image(
	input efi_file_t* root_file_system,
	input wuchar* kernel_image_filename,
	Elf64 *elf_object
) {
	efi_file_t *kernel_img = NULL;
	if (EFI_ERROR(root_file_system->open(root_file_system, &kernel_img, (wchar*)kernel_image_filename, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY))) {
		printf("Can't find kernel\r\n");
		return;
	} else {
		printf("Found kernel\r\n");
	}

	uint64 size = 64;
	kernel_img->read(kernel_img, &size, &elf_object->head);

	efi_status s = 0;

	if (!memcmp(elf_object->head.e_ident.ei_magic, Elf_Ident_Magic, 4) && 
		elf_object->head.e_type == ET_EXEC && 
		elf_object->head.e_machine == EM_X86_64
	) {
		printf("Valid elf executable\r\n");
		printf("ehdr:\r\n  ehdr.e_type = ET_EXEC\r\n  ehdr.e_machine = EM_X86_64\r\n");
	} else {
		printf("Invalid elf file\r\n");
		return;
	}

	size = elf_object->head.e_phnum * elf_object->head.e_phentsize;
	global.boot->memory.allocate_pool(EFI_LOADER_DATA, size, (void*)&elf_object->body.phdr);
	s = kernel_img->read(kernel_img, &size, elf_object->body.phdr);
	ON_ERR(s,return, "Can't load program header\r\n");
	printf("sizeof phdr: %ld = %d * %d\r\n", size, elf_object->head.e_phnum, elf_object->head.e_phentsize);

	for (uint16 i = 0; i < elf_object->head.e_phnum; i++) {
		Elf64_phdr_t phdr = elf_object->body.phdr[i];
		if (phdr.p_type == PT_LOAD) {
			load_segment(kernel_img, &phdr);
		}
	}
	printf("Done all phdr\r\n");
	kernel_img->close(kernel_img);

	printf("Kernel loaded\r\n");
}
