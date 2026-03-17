#include <uart.h>

typedef struct s_memory_region_desc {
	uint32_t type;
	uint32_t pad;
	uint64_t *physical_start;
	uint64_t *virtual_start;
	uint64_t count;
	uint64_t attributes;
} Memory_Map_Descriptor;

typedef struct s_boot_video_info {
	uint32_t* framebuffer_pointer;
	uint32_t horizontal_resolution;
	uint32_t vertical_resolution;
	uint32_t pixels_per_scanline;
} Kernel_Boot_Video_Mode_Info;

#include <vga.h>
/**
 * @brief Boot info struct.
 * Contains information passed to the kernel at boot time by the bootloader.
 */
typedef struct s_boot_info {
	Memory_Map_Descriptor* memory_map;
	uint64_t mmap_size;
	uint64_t mmap_descriptor_size;
	Kernel_Boot_Video_Mode_Info video_mode_info;
} Boot_Info;

void kernel_main(Boot_Info *boot) {
	char name[] = "Kernel loaded\r\nHello Jaipal from 64-bit\r\nNew line 3";

	uart_initialize();
	vga_puts(name);
	while(1) {
		asm("hlt"); 
	}
}

long strlen(char *s) {
	long res = 0;
	while (s[res]) {
		res++;
	}
	return res;
}
