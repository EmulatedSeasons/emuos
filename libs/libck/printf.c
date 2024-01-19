#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

// for %d and %x.
static const char* itoa(unsigned int num, char* str, unsigned int base) {
	int cou = 0; // nt

    if (num == 0) { // 0
		str[cou] = '0';
		str[++cou] = '\0';
		return str;
	}

	while (num)
	{
		unsigned int rem = (num % base);
		// brings the number up to ascii 0 + the digit
		if (rem > 9) {
			str[cou] = rem + '7'; // 7 = 55 + 10(A) = 65
		} else {
			str[cou] = rem + '0';
		}
		num /= base;
		++cou;
	}
	
	// reverse the string
	for (int i = 0; i < cou; i++) {
		char temp = str[i];
		str[i] = str[cou - i - 1]; 
		str[cou - 1] = temp;
	}

	str[cou] = '\0';
	return str;
}

// have no idea if there's a better way to do this. for %lx
static const char* itoa_u64(unsigned long long num, char* str, int base) {
	int cou = 0; // nt

    if (num == 0) { // 0
		str[cou] = '0';
		str[++cou] = '\0';
		return str;
	}

	while (num)
	{
		unsigned long long rem = num % base;
		// brings the number up to ascii 0 + the digit
		if (rem > 9) {
			str[cou] = rem + '7'; // 7 = 55 + 10(A) = 65
		} else {
			str[cou] = rem + '0';
		}
		num /= base;
		++cou;
	}
	
	// reverse the string
	for (int i = 0; i < cou; i++) {
		char temp = str[i];
		str[i] = str[cou - i - 1]; 
		str[cou - 1] = temp;
	}

	str[cou] = '\0';
	return str;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;
 
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 'd') {
			format++;
			char str[20]; // probably a long enough buffer
			unsigned int d = va_arg(parameters, unsigned int);
			itoa(d, str, 10);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'x') {
			format++;
			char str[20]; // probably a long enough buffer
			unsigned int d = va_arg(parameters, unsigned int);
			itoa(d, str, 16);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'l') { // for %lx and others that start with l
			format++;
			if (*format == 'x') {
				format++;
				char str[20]; // probably a long enough buffer
				unsigned long long d = va_arg(parameters, unsigned long long);
				itoa_u64(d, str, 16);
				size_t len = strlen(str);
				if (maxrem < len) {
					// TODO: Set errno to EOVERFLOW.
					return -1;
				}
				if (!print(str, len))
					return -1;
				written += len;
			}
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}