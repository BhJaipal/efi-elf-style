#include <efi-lib.h>


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
