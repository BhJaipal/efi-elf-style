#ifndef EFI_ELF_STRING_H
#define EFI_ELF_STRING_H
#include "types.h"

void char8_to_char16(int16 *dest, char *src, uint64 n);
void uchar8_to_uchar16(uint16 *dest, unsigned char *src, uint64 n);

#endif // !EFI_ELF_STRING_H
