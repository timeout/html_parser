#ifndef __ATTR_READER_INCLUDED__
#define __ATTR_READER_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include "html_parser_text.h"
#include "html_parser_attribute_list.h"

extern Attr_list_T attribute_reader(Text_T *tag);

#ifdef __cplusplus
}
#endif
#endif /* __ATTR_READER_INCLUDED__ */
