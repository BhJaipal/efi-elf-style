#include "efi-api.h"
#include "efi-bind.h"
#include "types.h"
#include <efi-lib.h>
#include <efi-err.h>

Efi_lib_flags_t efi_flags = {0};
global_t global = {0};

void initialize_lib(efi_handle_t ImageHandle, efi_system_table_t *SystemTable) {
	if (efi_flags.lib_initialized) return;

    global.sys = SystemTable;
    global.boot = SystemTable->boot_services;
    global.runtime = SystemTable->runtime_services;

	if (ImageHandle) {
		// efi_debug_variable();
	}
}
// NOTE: put it in efi-debug.h
uint64     efi_debug;

efi_guid_t efi_global_variable;
void efi_debug_variable() {
	efi_status status;
	uint32 attributes;
	uint64 data_size;
	uint64 new_efi_debug;

	data_size = sizeof(efi_debug);
	status = uefi_call_wrapper(global.runtime, 5, u"EFIDebug", &efi_global_variable, &attributes, &data_size, &new_efi_debug);
	if (EFI_ERROR(status)) {
		efi_debug = new_efi_debug;
	}
}


void shutdown() {
	global.runtime->reset_system(EFI_RESET_SHUTDOWN, 0, 0, NULL);
	__builtin_unreachable();
}

void exit(efi_status exit_status, int64 data_size, int16 *data) {
	global.runtime->reset_system(EFI_RESET_SHUTDOWN, exit_status, data_size, data);
	__builtin_unreachable();
}
