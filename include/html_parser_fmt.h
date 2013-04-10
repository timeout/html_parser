#ifndef __FMT_INCLUDED__
#define __FMT_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdio.h>

#include "html_parser_except.h"

typedef struct va_list_box {
	va_list ap;
} va_list_box;

#define T Fmt_T

typedef void (*T)(int code, va_list_box *box,
		int put(int c, void *cl), void *cl,
		unsigned char flags[256], int width, int precision);
extern char *Fmt_flags;
extern const Except_T Fmt_Overflow;
extern void Fmt_fmt (int put(int c, void *cl), void *cl,
		const char *fmt, ...);
extern void Fmt_vfmt(int put(int c, void *cl), void *cl,
		const char *fmt, va_list_box *box);
extern void Fmt_print (const char *fmt, ...);
extern void Fmt_fprint(FILE *stream,
		const char *fmt, ...);
extern int Fmt_sfmt   (char *buf, int size,
		const char *fmt, ...);
extern int Fmt_vsfmt(char *buf, int size,
		const char *fmt, va_list_box *box);
extern char *Fmt_string (const char *fmt, ...);
extern char *Fmt_vstring(const char *fmt, va_list_box *box);
extern T Fmt_register(int code, T cvt);
extern void Fmt_putd(const char *str, int len,
		int put(int c, void *cl), void *cl,
		unsigned char flags[256], int width, int precision);
extern void Fmt_puts(const char *str, int len,
		int put(int c, void *cl), void *cl,
		unsigned char flags[256], int width, int precision);

#undef T
#ifdef __cplusplus
}
#endif
#endif
