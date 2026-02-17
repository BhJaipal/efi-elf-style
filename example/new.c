#include "efi-api.h"
#include "efi-con.h"
#include "types.h"
#include <efi-string.h>
#include <efi-err.h>


efi_status efi_main(efi_handle_t img_handle, efi_system_table_t *system_table) {
	uint16 msg[] = u"Hello world\n";
	system_table->stdout->set_attribute(system_table->stdout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_GREEN));
	system_table->stdout->clear_screen(system_table->stdout);
	system_table->stdout->output_string(system_table->stdout, (int16*)msg);

	while (true) {
		asm("hlt");
	}
	return EFI_SUCCESS;
}
