#include "efi-api.h"
#include "efi-con.h"
#include "types.h"
#include <efi-string.h>
#include <efi-err.h>

extern void InitializeLib(efi_handle_t ImageHandle, efi_system_table_t *SystemTable);
extern void Print(int16 *fmt, ...);

efi_status efi_main(efi_handle_t img_handle, efi_system_table_t *system_table) {
	uint16 msg[] = u"Hello world\n";
	system_table->stdout->set_attribute(system_table->stdout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_GREEN));
	system_table->stdout->clear_screen(system_table->stdout);
	system_table->stdout->output_string(system_table->stdout, (int16*)msg);

	efi_input_key_t key;
	while (system_table->stdin->read_key_stroke(system_table->stdin, &key) != EFI_SUCCESS);

	system_table->runtime_services->reset_system(EFI_RESET_SHUTDOWN, EFI_SUCCESS, 0, NULL);
	return EFI_SUCCESS;
}
