#include "types.h"
#include <efi-lib.h>

#define VA __va_list_tag*

uint64 print_str(char* str, simple_text_output_interface_t *out);
uint64 print_wstr(short* str, simple_text_output_interface_t *out);
uint64 print_long(uint64 val, simple_text_output_interface_t *out);
uint64 print_ptr(void *val, simple_text_output_interface_t *out);
/*
 * @precision: float 7 and double 15 precision
 */
uint64 print_double(double val, char precision, simple_text_output_interface_t *out);

uint64 printf(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	uint64 out = vprint((unsigned char*)fmt, args, global.sys->cout);
	va_end(args);
	return out;
}
uint64 wprintf(unsigned short *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	uint64 out = vwprint(fmt, args, global.sys->cout);
	va_end(args);
	return out;
}

uint64 errorf(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	uint64 out = vprint((unsigned char*)fmt, args, global.sys->stderr);
	va_end(args);
	return out;
}
uint64 werrorf(unsigned short *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	uint64 out = vwprint(fmt, args, global.sys->stderr);
	va_end(args);
	return out;
}

uint64 vprint(unsigned char* str, va_list args, simple_text_output_interface_t *out) {
	short buff[256] = {0};
	uint64 res_len = 0;
	unsigned char n = 0;

	char c = *str;
	while (c) {
		if (c != '%') {
			if (n == 255) {
				out->output_string(out, buff);
				n = 0;
			}
			buff[n++] = c;
			buff[n] = 0;
			res_len++;

			c = *(++str);
			continue;
		}
		if (n) {
			out->output_string(out, buff);
			n = 0;
		}
		switch (str[1]) {
			case 'l':
				if (str[2] == 's') {
					res_len += print_wstr(va_arg(args, short*), out);
				} else if (str[2] == 'd') {
					res_len += print_long(va_arg(args, uint64), out);
				} else if (str[2] == 'f') {
					res_len += print_double(va_arg(args, double), 15, out);
				}
				str += 1;
				break;
			case 's':
				res_len += print_str(va_arg(args, char*), out);
				break;
			case 'f':
				res_len += print_double(va_arg(args, double), 7, out);
				break;
			case 'c':
				res_len++;
				short str__c[2] = {(char)va_arg(args, int), 0};
				out->output_string(out, str__c);
				break;
			case 'd':
				res_len += print_long(va_arg(args, uint32), out);
				break;
			case 'p':
				res_len += print_ptr(va_arg(args, void*), out);
				break;
			case '%':
				out->output_string(out, (short*)u"%");
				n++;
				res_len++;
				break;
			default:
				out->output_string(out, (short*)u"%");
				res_len++;
				if (str[1]) {
					int s = str[1];
					s &= 0x0000ffff;
					out->output_string(out, (short*)&s);
					str--;
					res_len++;
				}
		}
		str += 2;
		c = *str;
	}
	if (n) {
		out->output_string(out, buff);
		n = 0;
	}
	return res_len;
}
uint64 vwprint(unsigned short* str, va_list args, simple_text_output_interface_t *out) {
	short buff[256] = {0};
	uint64 res_len = 0;
	unsigned char n = 0;

	short c = *str;
	while (c) {
		if (c != '%') {
			if (n == 255) {
				out->output_string(out, buff);
				n = 0;
			}
			buff[n++] = c;
			buff[n] = 0;
			res_len++;

			c = *(++str);
			continue;
		}
		if (n) {
			out->output_string(out, buff);
			n = 0;
		}
		switch (str[1]) {
			case 'l':
				if (str[2] == 's') {
					res_len += print_wstr(va_arg(args, short*), out);
				} else if (str[2] == 'd') {
					res_len += print_long(va_arg(args, uint64), out);
				} else if (str[2] == 'f') {
					res_len += print_double(va_arg(args, double), 15, out);
				}
				str += 1;
				break;
			case 's':
				res_len += print_str(va_arg(args, char*), out);
				break;
			case 'd':
				res_len += print_long(va_arg(args, uint32), out);
				break;
			case 'p':
				res_len += print_ptr(va_arg(args, void*), out);
				break;
			case 'f':
				res_len += print_double(va_arg(args, double), 7, out);
				break;
			case 'c':
				res_len++;
				short str__c[2] = {(char)va_arg(args, int), 0};
				out->output_string(out, (str__c));
				break;
			case '%':
				out->output_string(out, (short*)u"%");
				res_len++;
				n++;
				break;
			default:
				out->output_string(out, (short*)u"%");
				res_len++;
				if (str[1]) {
					res_len++;
					int s = str[1];
					s &= 0x0000ffff;
					out->output_string(out, (short*)&s);
					str--;
				}
		}
		str += 2;
		c = *str;
	}
	if (n) {
		out->output_string(out, buff);
		n = 0;
	}
	return res_len;
}

uint64 print_str(char* str, simple_text_output_interface_t *out) {
	uint64 res_len = 0;

	char c = *str;
	while (c) {
		int s = c;
		s &= 0x0000ffff;
		out->output_string(out, (short*)&s);
		res_len++;
		c = *(++str);
	}
	return res_len;
}


uint64 print_wstr(short* str, simple_text_output_interface_t *out) {
	uint64 res_len = 0;

	int c = *str;
	while (c) {
		c &= 0x0000ffff;
		out->output_string(out, (short*)&c);
		res_len++;
		c = *(++str);
	}
	return res_len;
}

uint64 print_long(uint64 val, simple_text_output_interface_t *out) {
	if (!val) {
		out->output_string(out, (short*)u"0");
		return 1;
	}
	uint64 len = 0;
	if (val < 0) {
		val = -val;
		out->output_string(out, (short*)u"-");
		len++;
	}

	char zeros = 0;
	uint64 rev = 0;
	uint64 x = val;
	while (!(x % 10)) {
		x /= 10;
		zeros++;
	}
	while (val) {
		rev *= 10;
		rev += val % 10;
		val /= 10;
		len++;
	}

	int s = 0;
	while (rev) {
		s = (rev % 10) + 0x30;
		s &= 0x000000ff;
		rev /= 10;
		out->output_string(out, (short*)&s);
	}
	for (char i = 0; i < zeros; i++) {
		out->output_string(out, (short*)u"0");
	}
	return len;
}

/*
 * @precision: float 7 and double 15 precision
 */
uint64 print_double(double val, char precision, simple_text_output_interface_t *out) {
	if (!val) {
		out->output_string(out, (short*)u"0");
		return 1;
	}
	uint64 floor_val = (int)val;
	uint64 res_len = print_long(floor_val, out);
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
	out->output_string(out, (short*)u".");

	while (precision--) {
		diff *= 10;
		int digit = (int)diff;
		diff -= digit;
		int s = digit + 0x30;
		s &= 0x0000ffff;
		out->output_string(out, (short*)&s);
		res_len++;
	}

	return res_len;
}
uint64 print_ptr(void *val_, simple_text_output_interface_t *out) {
	out->output_string(out, (short*)u"0x");
	if (val_ == NULL) {
		out->output_string(out, (wchar*)u"0");
		return 3;
	}

	uint64 len = 2;
	uint64 val = (uint64)val_;
	uint64 ptr = 0;
	char zeros = 0;
	
	while (!(val & 0xf)) {
		zeros++;
		val >>= 4;
	}
	while (val) {
		ptr <<= 4;
		ptr |= val & 0xf;
		val >>= 4;
	}

	wchar buff[2] = {0};
	while (ptr) {
		char chr = ptr & 0xf;
		if (chr < 10) {
			buff[0] = chr + 0x30;
		} else {
			buff[0] = chr - 10 + 0x61;
		}
		out->output_string(out, buff);
		len++;

		ptr >>= 4;
	}

	for (char i = 0; i < zeros; i++) {
		out->output_string(out, (wchar*)u"0");
	}
	len += zeros;
	return len;
}

