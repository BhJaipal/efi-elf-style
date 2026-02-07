#ifndef EFI_ELF_EFI_BIND
#define EFI_ELF_EFI_BIND

#include <efi/x86_64/efibind.h>

#undef EFI_DRIVER_ENTRY_POINT
#define EFI_DRIVER_ENTRY_POINT(init_driver_func)    \
        UINTN                                       \
        InitializeDriver (                          \
            VOID    *ImageHandle,                   \
            VOID    *SystemTable                    \
            )                                       \
        {                                           \
            return InitFunction(ImageHandle,        \
                    SystemTable);                   \
        }                                           \
                                                    \
        EFI_STATUS efi_main(                        \
            EFI_HANDLE image,                       \
            EFI_SYSTEM_TABLE *systab                \
            ) __attribute__((weak,                  \
                    alias ("InitializeDriver")));


// Simpler and better, stdcall wrapper implemented with using some brain
#undef uefi_call_wrapper
#define uefi_call_wrapper(func,va_num,...)              \
	_cast64_efi_call##va_num(func, __VA_ARGS__);


#endif // !EFI_ELF_EFI_BIND
