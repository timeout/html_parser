#ifndef __FILE_READER_H__
#define __FILE_READER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "html_parser_text.h"

Text_T *File_reader_reader(const char *pathname, const char *filter);

#ifdef __cplusplus
}
#endif

#endif /* __FILE_READER_H__ */
