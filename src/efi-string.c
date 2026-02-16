#include <types.h>

void char8_to_char16(int16 *dest, char *src, uint64 n) {
	for (uint64 i = 0; i < n; i++) {
		dest[i] = (int16)src[i];
	}
}

void uchar8_to_uchar16(uint16 *dest, unsigned char *src, uint64 n) {
	for (uint64 i = 0; i < n; i++) {
		dest[i] = (uint16)src[i];
	}
}
