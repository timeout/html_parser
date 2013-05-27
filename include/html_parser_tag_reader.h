#ifndef __TAG_READER_INCLUDED__
#define __TAG_READER_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include "html_parser_types.h"
#include "html_parser_text.h"
#include "html_parser_tag_lookup.h"

extern Tag_E Tag_reader_reader(Text_T *tag, char **ch, Lookup_T *tbl);

#ifdef __cplusplus
}
#endif
#endif /* __TAG_READER_INCLUDED__ */
