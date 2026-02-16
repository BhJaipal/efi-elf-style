#ifndef EFI_ELF_EFI_BIND
#define EFI_ELF_EFI_BIND

#include "efi-api.h"

#undef EFI_DRIVER_ENTRY_POINT
#define EFI_DRIVER_ENTRY_POINT(init_driver_func)    \
        uint64                                       \
        InitializeDriver (                          \
            void    *image_handle,                  \
            void    *system_table                   \
            )                                       \
        {                                           \
            return init_driver_func(image_handle,   \
                    system_table);                  \
        }                                           \
                                                    \
        efi_status efi_main(                        \
            efi_handle_t image,                     \
            efi_system_table_t *systab              \
            ) __attribute__((weak,                  \
                    alias ("InitializeDriver")));


#define EFIERR(a)           (0x8000000000000000 | a)
#define EFI_ERROR_MASK      0x8000000000000000
#define EFIERR_OEM(a)       (0xc000000000000000 | a)      


#define BAD_POINTER         0xFBFBFBFBFBFBFBFB
#define MAX_ADDRESS         0xFFFFFFFFFFFFFFFF
#define BREAKPOINT()        while (true);    // Make it hang on Bios[Dbg]32

#define MIN_ALIGNMENT_SIZE  4

#define ALIGN_VARIABLE(value, adjustment) \
            (uint64)adjustment = 0; \
            if((uint64)value % MIN_ALIGNMENT_SIZE) \
                (uint64)adjustment = MIN_ALIGNMENT_SIZE - ((uint64)value % MIN_ALIGNMENT_SIZE); \
            value = (uint64)value + (uint64)adjustment

// Simpler and better, stdcall wrapper implemented with using some brain
#undef uefi_call_wrapper
#define uefi_call_wrapper(func,va_num,...)              \
	cast64_efi_call##va_num(func, __VA_ARGS__);


/* Prototypes of EFI cdecl -> stdcall trampolines */
uint64 efi_call0(void *func);
uint64 efi_call1(void *func, uint64 arg1);
uint64 efi_call2(void *func, uint64 arg1, uint64 arg2);
uint64 efi_call3(void *func, uint64 arg1, uint64 arg2, uint64 arg3);
uint64 efi_call4(void *func, uint64 arg1, uint64 arg2, uint64 arg3,
                 uint64 arg4);
uint64 efi_call5(void *func, uint64 arg1, uint64 arg2, uint64 arg3,
                 uint64 arg4, uint64 arg5);
uint64 efi_call6(void *func, uint64 arg1, uint64 arg2, uint64 arg3,
                 uint64 arg4, uint64 arg5, uint64 arg6);
uint64 efi_call7(void *func, uint64 arg1, uint64 arg2, uint64 arg3,
                 uint64 arg4, uint64 arg5, uint64 arg6, uint64 arg7);
uint64 efi_call8(void *func, uint64 arg1, uint64 arg2, uint64 arg3,
                 uint64 arg4, uint64 arg5, uint64 arg6, uint64 arg7,
                 uint64 arg8);
uint64 efi_call9(void *func, uint64 arg1, uint64 arg2, uint64 arg3,
                 uint64 arg4, uint64 arg5, uint64 arg6, uint64 arg7,
                 uint64 arg8, uint64 arg9);
uint64 efi_call10(void *func, uint64 arg1, uint64 arg2, uint64 arg3,
                  uint64 arg4, uint64 arg5, uint64 arg6, uint64 arg7,
                  uint64 arg8, uint64 arg9, uint64 arg10);

/* Front-ends to efi_callX to avoid compiler warnings */
#define cast64_efi_call0(f) \
  efi_call0(f)
#define cast64_efi_call1(f,a1) \
  efi_call1(f, (uint64)(a1))
#define cast64_efi_call2(f,a1,a2) \
  efi_call2(f, (uint64)(a1), (uint64)(a2))
#define cast64_efi_call3(f,a1,a2,a3) \
  efi_call3(f, (uint64)(a1), (uint64)(a2), (uint64)(a3))
#define cast64_efi_call4(f,a1,a2,a3,a4) \
  efi_call4(f, (uint64)(a1), (uint64)(a2), (uint64)(a3), (uint64)(a4))
#define cast64_efi_call5(f,a1,a2,a3,a4,a5) \
  efi_call5(f, (uint64)(a1), (uint64)(a2), (uint64)(a3), (uint64)(a4), \
            (uint64)(a5))
#define cast64_efi_call6(f,a1,a2,a3,a4,a5,a6) \
  efi_call6(f, (uint64)(a1), (uint64)(a2), (uint64)(a3), (uint64)(a4), \
            (uint64)(a5), (uint64)(a6))
#define cast64_efi_call7(f,a1,a2,a3,a4,a5,a6,a7) \
  efi_call7(f, (uint64)(a1), (uint64)(a2), (uint64)(a3), (uint64)(a4), \
            (uint64)(a5), (uint64)(a6), (uint64)(a7))
#define cast64_efi_call8(f,a1,a2,a3,a4,a5,a6,a7,a8) \
  efi_call8(f, (uint64)(a1), (uint64)(a2), (uint64)(a3), (uint64)(a4), \
            (uint64)(a5), (uint64)(a6), (uint64)(a7), (uint64)(a8))
#define cast64_efi_call9(f,a1,a2,a3,a4,a5,a6,a7,a8,a9) \
  efi_call9(f, (uint64)(a1), (uint64)(a2), (uint64)(a3), (uint64)(a4), \
            (uint64)(a5), (uint64)(a6), (uint64)(a7), (uint64)(a8), \
            (uint64)(a9))
#define cast64_efi_call10(f,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) \
  efi_call10(f, (uint64)(a1), (uint64)(a2), (uint64)(a3), (uint64)(a4), \
             (uint64)(a5), (uint64)(a6), (uint64)(a7), (uint64)(a8), \
             (uint64)(a9), (uint64)(a10))

#endif // !EFI_ELF_EFI_BIND
