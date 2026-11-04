#ifndef ARDUTIL_H
#define ARDUTIL_H

#ifndef DEC
#define DEC 10
#endif

#include <ctype.h>

#ifdef __cplusplus
#include <algorithm>
#else
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

unsigned long millis(void);
void delay(unsigned long ms);
bool isHexadecimalDigit(int character);
bool isSpace(int character);
char *itoa(int value, char *buffer, int base);

static inline bool isAlphaNumeric(int character)
{
	return isalnum((unsigned char)character) != 0;
}

#ifdef __cplusplus
}

using std::max;
using std::min;
#endif

#endif