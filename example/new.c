#include "efi-api.h"
#include "efi-con.h"
#include "types.h"
#include <efi-string.h>
#include <efi-err.h>


efi_status efi_main(efi_handle_t img_handle, efi_system_table_t *system_table) {
	uint16 msg[] = u"Hello world\r\nPress Q to exit\r\n";
	system_table->cout->set_attribute(system_table->cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_GREEN));
	system_table->cout->clear_screen (system_table->cout);
	system_table->cout->output_string(system_table->cout, (int16*)msg);

	efi_input_key_t key;
	while (true) {
		if (!system_table->cin->read_key_stroke(system_table->cin, &key)) {
			uint16 msg[] = u"Current input: X\r";
			msg[15] = key.unicode_char;
			system_table->cout->output_string(system_table->cout, (int16*)msg);
			if (key.unicode_char == 'Q' || key.unicode_char == 'q') {
				system_table->runtime_services->reset_system(EFI_RESET_SHUTDOWN, 0, 0, NULL);
			}
		}
		asm("hlt");
	}
	return EFI_SUCCESS;
}
