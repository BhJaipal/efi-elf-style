#ifndef EFI_ELF_EFI_LIB
#define EFI_ELF_EFI_LIB

#include "efi-api.h"

#ifndef va_list
typedef __builtin_va_list va_list;
# define va_start(v,l)  __builtin_va_start(v,l)
# define va_end(v)      __builtin_va_end(v)
# define va_arg(v,l)    __builtin_va_arg(v,l)
# define va_copy(d,s)   __builtin_va_copy(d,s)
typedef struct __va_list_tag __va_list_tag;
#endif // !va_list

typedef struct Efi_lib_flags_t {
	char lib_initialized: 1;
} Efi_lib_flags_t;


typedef struct {
	efi_system_table_t      *sys;
	efi_boot_services_t     *boot;
	efi_runtime_services_t  *runtime;
} global_t;


extern Efi_lib_flags_t efi_flags;
extern global_t global;

extern efi_guid_t efi_global_variable;


// NOTE: put it in efi-debug.h
extern uint64     efi_debug;
extern void initialize_lib(efi_handle_t ImageHandle, efi_system_table_t *SystemTable);
extern void efi_debug_variable();
extern long vprint(unsigned char*, va_list args);
extern long printf(const char *fmt, ...);

extern long vwprint(unsigned short*, va_list args);

extern long wprintf(unsigned short *fmt, ...);

// Exit the system
extern void exit(efi_status exit_status, int64 data_size, int16 *data);

// Exit with all other args 0 and NULL
extern void shutdown();

#endif // !EFI_ELF_EFI_LIB
