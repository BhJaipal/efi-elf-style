#include <efi-elf.h>
#include <efi-err.h>
#include <efi-lib.h>

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
	efi_physical_addr_t zero_fill_start = phdr->p_phy_addr + phdr->p_filesz;

	if (zero_fill_count) {
		global.boot->set_mem((void*)zero_fill_start, zero_fill_count, 0);
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
