#include "efi-api.h"
#include "types.h"
#include <efi-lib.h>
#define VA __va_list_tag*

long print_str(char* str);
long print_wstr(short* str);
long print_long(long val);
/*
 * @precision: float 7 and double 15 precision
 */
long print_double(double val, char precision);


// Global variables to hold the pointers
static efi_system_table_t *ST = NULL;
static efi_boot_services_t *BS = NULL;

void initialize_lib(efi_handle_t ImageHandle, efi_system_table_t *SystemTable) {
    ST = SystemTable;
    BS = SystemTable->boot_services;
}

long printf(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	long out = vprint((unsigned char*)fmt, args);
	va_end(args);
	return out;
}
long wprintf(unsigned short *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	long out = vwprint(fmt, args);
	va_end(args);
	return out;
}

long vprint(unsigned char* str, va_list args) {
	short buff[256] = {0};
	long res_len = 0;
	unsigned char n = 0;

	char c = *str;
	while (c) {
		if (c != '%') {
			if (n == 255) {
				ST->cout->output_string(ST->cout, buff);
				n = 0;
			}
			buff[n++] = c;
			buff[n] = 0;
			res_len++;

			c = *(++str);
			continue;
		}
		if (n) {
			ST->cout->output_string(ST->cout, buff);
			n = 0;
		}
		switch (str[1]) {
			case 'l':
				if (str[2] == 's') {
					res_len += print_wstr(va_arg(args, short*));
				} else if (str[2] == 'd') {
					res_len += print_long(va_arg(args, long));
				} else if (str[2] == 'f') {
					res_len += print_double(va_arg(args, double), 15);
				}
				str += 1;
				break;
			case 's':
				res_len += print_str(va_arg(args, char*));
				break;
			case 'f':
				res_len += print_double(va_arg(args, double), 7);
				break;
			case 'd':
				res_len += print_long(va_arg(args, long));
				break;
			case '%':
				ST->cout->output_string(ST->cout, (short*)u"%");
				n++;
				res_len++;
				break;
			default:
				ST->cout->output_string(ST->cout, (short*)u"%");
				res_len++;
				if (str[1]) {
					int s = str[1];
					s &= 0x0000ffff;
					ST->cout->output_string(ST->cout, (short*)&s);
					str--;
					res_len++;
				}
		}
		str += 2;
		c = *str;
	}
	if (n) {
		ST->cout->output_string(ST->cout, buff);
		n = 0;
	}
	return res_len;
}
long vwprint(unsigned short* str, va_list args) {
	short buff[256] = {0};
	long res_len = 0;
	unsigned char n = 0;

	short c = *str;
	while (c) {
		if (c != '%') {
			if (n == 255) {
				ST->cout->output_string(ST->cout, buff);
				n = 0;
			}
			buff[n++] = c;
			buff[n] = 0;
			res_len++;

			c = *(++str);
			continue;
		}
		if (n) {
			ST->cout->output_string(ST->cout, buff);
			n = 0;
		}
		switch (str[1]) {
			case 'l':
				if (str[2] == 's') {
					res_len += print_wstr(va_arg(args, short*));
				} else if (str[2] == 'd') {
					res_len += print_long(va_arg(args, long));
				} else if (str[2] == 'f') {
					res_len += print_double(va_arg(args, double), 15);
				}
				str += 1;
				break;
			case 's':
				res_len += print_str(va_arg(args, char*));
				break;
			case 'd':
				res_len += print_long(va_arg(args, long));
				break;
			case 'f':
				res_len += print_double(va_arg(args, double), 7);
				break;
			case '%':
				ST->cout->output_string(ST->cout, (short*)u"%");
				res_len++;
				n++;
				break;
			default:
				ST->cout->output_string(ST->cout, (short*)u"%");
				res_len++;
				if (str[1]) {
					res_len++;
					int s = str[1];
					s &= 0x0000ffff;
					ST->cout->output_string(ST->cout, (short*)&s);
					str--;
				}
		}
		str += 2;
		c = *str;
	}
	if (n) {
		ST->cout->output_string(ST->cout, buff);
		n = 0;
	}
	return res_len;
}

long print_str(char* str) {
	long res_len = 0;

	char c = *str;
	while (c) {
		int s = c;
		s &= 0x0000ffff;
		ST->cout->output_string(ST->cout, (short*)&s);
		res_len++;
		c = *(++str);
	}
	return res_len;
}


long print_wstr(short* str) {
	long res_len = 0;

	int c = *str;
	while (c) {
		c &= 0x0000ffff;
		ST->cout->output_string(ST->cout, (short*)&c);
		res_len++;
		c = *(++str);
	}
	return res_len;
}

long print_long(long val) {
	if (!val) {
		ST->cout->output_string(ST->cout, (short*)u"0");
		return 1;
	}
	long len = 0;
	if (val < 0) {
		val = -val;
		ST->cout->output_string(ST->cout, (short*)u"-");
		len++;
	}

	char zeros = 0;
	long rev = 0;
	while (val) {
		rev *= 10;
		rev += val % 10;
		if (!(val % 10)) zeros++;
		val /= 10;
		len++;
	}

	int s = 0;
	while (rev % 10) {
		s = (rev % 10) + 0x30;
		s &= 0x0000ffff;
		rev /= 10;
		ST->cout->output_string(ST->cout, (short*)&s);
	}
	for (char i = 0; i < zeros; i++) {
		ST->cout->output_string(ST->cout, (short*)u"0");
	}
	return len;
}

/*
 * @precision: float 7 and double 15 precision
 */
long print_double(double val, char precision) {
	if (!val) {
		ST->cout->output_string(ST->cout, (short*)u"0");
		return 1;
	}
	long floor_val = (int)val;
	long res_len = print_long(floor_val);
	if (val < 0) {
		val = -val;
		floor_val = (int)val;
	}

	// No decimal value
	double diff = val - (double)floor_val;
	if (diff < 0.000001) {
		return res_len;
	}
	// Decimal
	res_len++;
	ST->cout->output_string(ST->cout, (short*)u".");

	while (precision--) {
		diff *= 10;
		int digit = (int)diff;
		diff -= digit;
		int s = digit + 0x30;
		s &= 0x0000ffff;
		ST->cout->output_string(ST->cout, (short*)&s);
		res_len++;
	}

	return res_len;
}

void shutdown() {
	ST->runtime_services->reset_system(EFI_RESET_SHUTDOWN, 0, 0, NULL);
}
