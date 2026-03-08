#include "types.h"
#include <efi-err.h>
#include <efi-lib.h>
#include <efi-prot.h>
#include <efi-ser.h>
#include <efi-elf.h>

#define KERNEL_EXECUTABLE_PATH u"\\kernel.elf"

typedef struct s_boot_video_info {
	void    *framebuffer_pointer;
	uint32  horizontal_resolution;
	uint32  vertical_resolution;
	uint32  pixels_per_scanline;
} Kernel_Boot_Video_Mode_Info;

void *memcpy(void *dest, const void *src, uint64 n);

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

int memcmp(const void *dest, const void *src, uint64 n) {
	for (uint64 i = 0; i < n; i++) {
		if (((char*)dest)[i] != ((char*)src)[i])
			return 1;
	}
	return 0;
}

void load_segment(efi_file_t *kernel_img, Elf64_phdr_t phdr);

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

	efi_file_io_token_t kernel_io_token;
	kernel_io_token.buffer_size = 64;
	kernel_img->read_ex(kernel_img, &kernel_io_token);


	global.boot->memory.allocate_pool(EFI_LOADER_DATA, sizeof(Elf64), (void**)elf_object);
	memcpy(&elf_object->head, kernel_io_token.buffer, 64);

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

	efi_file_io_token_t section_token = {0}, program_token = {0}, content_token = {0};

	program_token.buffer_size = elf_object->head.e_phnum * elf_object->head.e_phentsize;
	content_token.buffer_size = elf_object->head.e_shoff - program_token.buffer_size - 64;
	section_token.buffer_size = elf_object->head.e_shnum * elf_object->head.e_shentsize;

	s = kernel_img->read_ex(kernel_img, &program_token);
	if (s < 0) {
		efi_error(s, "Can't load program header\r\n");
		return;
	}

	s = kernel_img->read_ex(kernel_img, &content_token);
	if (s < 0) {
		efi_error(s, "Can't load body\r\n");
		return;
	}

	s = kernel_img->read_ex(kernel_img, &section_token);
	if (s < 0) {
		efi_error(s, "Can't load section header\r\n");
		return;
	}

	elf_object->body.phdr = program_token.buffer;
	elf_object->body.body = content_token.buffer;
	elf_object->body.shdr = section_token.buffer;
	
	for (uint16 i = 0; i < elf_object->head.e_phnum; i++) {
		Elf64_phdr_t phdr = elf_object->body.phdr[i];
		if (phdr.p_type == PT_LOAD)
			load_segment(kernel_img, phdr);
	}
	printf("Done all phdr\r\n");
	kernel_img->close(kernel_img);

	printf("Kernel loaded\r\n");

	// void (*entry_point)() = (void(*)())(elf_object->body.shdr[2].sh_offset + content_token.buffer - 64 - program_token.buffer_size);
}

void *memcpy(void *dest, const void *src, uint64 n) {
	for (uint64 i = 0; i < n; i++) {
		((char*)dest)[i] = ((char*)src)[i];
	}
	return dest;
}

void load_segment(efi_file_t *kernel_img, Elf64_phdr_t phdr) {
	printf("Starting phdr\r\n");
	void *program_data = NULL;
	uint64 buffer_size = 0;
	uint64 page_segment_count = EFI_SIZE_TO_PAGES(phdr.p_memsz);

	kernel_img->set_position(kernel_img, phdr.p_filesz);
	global.boot->memory.allocate_pages(ALLOCATE_ADDRESS, EFI_LOADER_DATA, page_segment_count, &phdr.p_phy_addr);

	if (phdr.p_filesz) {
		global.boot->memory.allocate_pool(EFI_LOADER_CODE, buffer_size, &program_data);

		kernel_img->read(kernel_img, &buffer_size, program_data);
		global.boot->copy_mem((void*)phdr.p_phy_addr, program_data, phdr.p_filesz);
		global.boot->memory.free_pool(program_data);
	}
	printf("Ending phdr\r\n");
}

efi_status efi_main(efi_handle_t img_handle, efi_system_table_t *system_table) {
	system_table->cout->set_attribute(system_table->cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_GREEN));
	system_table->cout->clear_screen (system_table->cout);
	initialize_lib(img_handle, system_table);


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

