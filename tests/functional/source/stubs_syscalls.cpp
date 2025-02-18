// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstdio>
#include <cwchar>
#include <reent.h>

extern "C" {

// Stub implementation for putwc.
wint_t putwc(wchar_t ch, FILE *stream)
{
	(void)ch;
	(void)stream;
	return EOF;
}

// Stub implementation for getwc.
wint_t getwc(FILE *stream)
{
	(void)stream;
	return EOF;
}

// Stub implementation for ungetwc.
wint_t ungetwc(wint_t ch, FILE *stream)
{
	(void)ch;
	(void)stream;
	return EOF;
}

// Stub implementation for swprintf.
// This minimal version does nothing useful.
int swprintf(wchar_t *s, size_t n, const wchar_t *format, ...)
{
	(void)format;
	if (n > 0) {
		s[0] = L'\0';
	}
	return 0;
}
}
