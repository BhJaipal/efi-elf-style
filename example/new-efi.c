#include "efiapi.h"
#include "efidef.h"
#include "efierr.h"
#include "efiprot.h"
#include "x86_64/efibind.h"
#include <efi.h>

EFIAPI int efi_main(EFI_HANDLE img, EFI_SYSTEM_TABLE *sys) {
	EFI_GRAPHICS_OUTPUT_PROTOCOL k;
	return EFI_SUCCESS;
}
