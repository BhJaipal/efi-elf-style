#include <efi.h>
#include <efilib.h>

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    // Standard initialization
    InitializeLib(ImageHandle, SystemTable);

    // Use the ConsoleOut protocol to print to the screen
    // Note: UEFI uses UCS-2 (Wide) strings, hence the L prefix
	Print((CHAR16 *)L"Hello world\n");

    // Stay here so the message doesn't disappear immediately
    while(1);

    return EFI_SUCCESS;
}
