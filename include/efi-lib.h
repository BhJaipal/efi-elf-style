#ifndef EFI_ELF_EFI_LIB
#define EFI_ELF_EFI_LIB

#ifndef va_list
typedef __builtin_va_list va_list;
# define va_start(v,l)  __builtin_va_start(v,l)
# define va_end(v)      __builtin_va_end(v)
# define va_arg(v,l)    __builtin_va_arg(v,l)
# define va_copy(d,s)   __builtin_va_copy(d,s)
typedef struct __va_list_tag __va_list_tag;
#endif // !va_list


#include "efi-api.h"

extern void initialize_lib(efi_handle_t ImageHandle, efi_system_table_t *SystemTable);
extern long vprint(unsigned char*, va_list args);
extern long printf(const char *fmt, ...);

extern long vwprint(unsigned short*, va_list args);

void shutdown();
extern long wprintf(unsigned short *fmt, ...);

#endif // !EFI_ELF_EFI_LIB
