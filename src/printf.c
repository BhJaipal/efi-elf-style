#include "types.h"
#include <efi-err.h>
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

void efi_error(efi_status status, char *fmt, ...) {
	switch (status) {
		case EFI_SUCCESS:
			printf("[EFI_SUCCESS: ");
			break;
		case EFI_LOAD_ERROR:
			printf("EFI_LOAD_ERROR: ");
			break;
		case EFI_INVALID_PARAMETER:
			printf("[EFI_INVALID_PARAMETER: ");
			break;
		case EFI_UNSUPPORTED:
			printf("[EFI_UNSUPPORTED: ");
			break;
		case EFI_BAD_BUFFER_SIZE:
			printf("[EFI_BAD_BUFFER_SIZE: ");
			break;
		case EFI_BUFFER_TOO_SMALL:
			printf("[EFI_BUFFER_TOO_SMALL: ");
			break;
		case EFI_NOT_READY:
			printf("[EFI_NOT_READY: ");
			break;
		case EFI_DEVICE_ERROR:
			printf("[EFI_DEVICE_ERROR: ");
			break;
		case EFI_WRITE_PROTECTED:
			printf("[EFI_WRITE_PROTECTED: ");
			break;
		case EFI_OUT_OF_RESOURCES:
			printf("[EFI_OUT_OF_RESOURCES: ");
			break;
		case EFI_VOLUME_CORRUPTED:
			printf("[EFI_VOLUME_CORRUPTED: ");
			break;
		case EFI_VOLUME_FULL:
			printf("[EFI_VOLUME_FULL: ");
			break;
		case EFI_NO_MEDIA:
			printf("[EFI_NO_MEDIA: ");
			break;
		case EFI_MEDIA_CHANGED:
			printf("[EFI_MEDIA_CHANGED: ");
			break;
		case EFI_NOT_FOUND:
			printf("[EFI_NOT_FOUND: ");
			break;
		case EFI_ACCESS_DENIED:
			printf("[EFI_ACCESS_DENIED: ");
			break;
		case EFI_NO_RESPONSE:
			printf("[EFI_NO_RESPONSE: ");
			break;
		case EFI_NO_MAPPING:
			printf("[EFI_NO_MAPPING: ");
			break;
		case EFI_TIMEOUT:
			printf("[EFI_TIMEOUT: ");
			break;
		case EFI_NOT_STARTED:
			printf("[EFI_NOT_STARTED: ");
			break;
		case EFI_ALREADY_STARTED:
			printf("[EFI_ALREADY_STARTED: ");
			break;
		case EFI_ABORTED:
			printf("[EFI_ABORTED: ");
			break;
		case EFI_ICMP_ERROR:
			printf("[EFI_ICMP_ERROR: ");
			break;
		case EFI_TFTP_ERROR:
			printf("[EFI_TFTP_ERROR: ");
			break;
		case EFI_PROTOCOL_ERROR:
			printf("[EFI_PROTOCOL_ERROR: ");
			break;
		case EFI_INCOMPATIBLE_VERSION:
			printf("[EFI_INCOMPATIBLE_VERSION: ");
			break;
		case EFI_SECURITY_VIOLATION:
			printf("[EFI_SECURITY_VIOLATION: ");
			break;
		case EFI_CRC_ERROR:
			printf("[EFI_CRC_ERROR: ");
			break;
		case EFI_END_OF_MEDIA:
			printf("[EFI_END_OF_MEDIA: ");
			break;
		case EFI_END_OF_FILE:
			printf("[EFI_END_OF_FILE: ");
			break;
		case EFI_INVALID_LANGUAGE:
			printf("[EFI_INVALID_LANGUAGE: ");
			break;
		case EFI_COMPROMISED_DATA:
			printf("[EFI_COMPROMISED_DATA: ");
			break;
		case EFI_WARN_UNKOWN_GLYPH:
			printf("[EFI_WARN_UNKOWN_GLYPH: ");
			break;
		case EFI_WARN_DELETE_FAILURE:
			printf("[EFI_WARN_DELETE_FAILURE: ");
			break;
		case EFI_WARN_WRITE_FAILURE:
			printf("[EFI_WARN_WRITE_FAILURE: ");
			break;
		case EFI_WARN_BUFFER_TOO_SMALL:
			printf("[EFI_WARN_BUFFER_TOO_SMALL: ");
			break;
	}
	va_list args;
	va_start(args, fmt);
	vprint((unsigned char*)fmt, args, global.sys->cout);
	va_end(args);
}
void wefi_error(efi_status status, wuchar *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vwprint(fmt, args, global.sys->cout);
	va_end(args);
}
