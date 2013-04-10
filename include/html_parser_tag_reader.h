#ifndef __TAG_READER_INCLUDED__
#define __TAG_READER_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include "html_parser_text.h"

typedef enum tag_r_E {
	NO_ATTRIBUTES,
	TAG_READ, 
	COMMENT, 
	DIRECTIVE 
} tag_reader_E;

extern tag_reader_E tag_reader(Text_T *tag, char **ch);

#ifdef __cplusplus
}
#endif
#endif /* __TAG_READER_INCLUDED__ */
