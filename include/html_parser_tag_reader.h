#ifndef __TAG_READER_INCLUDED__
#define __TAG_READER_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include "html_parser_text.h"

typedef enum tag_reader_e {
	T_VOID = 1 << 1,
	T_CMMNT = 1 << 2,
	T_INSTR = 1 << 3,
	T_CLOSE = 1 << 4,
	T_YATTR = 1 << 5,
	T_NATTR = 1 << 6
} tag_E;

extern tag_E tag_reader(Text_T *tag, char **ch);

#ifdef __cplusplus
}
#endif
#endif /* __TAG_READER_INCLUDED__ */
