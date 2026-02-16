#include <efi-string.h>
#include <efi-err.h>

extern void InitializeLib(efi_handle_t ImageHandle, efi_system_table_t *SystemTable);
extern void Print(int16 *fmt, ...);

efi_status efi_main(efi_handle_t img_handle, efi_system_table_t *system_table) {
	int16 msg[20];
	InitializeLib(img_handle, system_table);
	char8_to_char16(msg, "Hello world\n", 13);
	Print(msg);
	system_table->stdout->output_string(system_table->stdout, msg);
	return EFI_SUCCESS;
}
