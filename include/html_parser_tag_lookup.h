#ifndef __TAG_LOOKUP_INCLUDED__
#define __TAG_LOOKUP_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include "html_parser_types.h"

#define T Lookup_T
typedef struct T *T;

extern T Tag_lookup_init(void);
extern Tag_E Tag_lookup_tag(T *t, const char *name);
extern const char *Tag_type_rep(Tag_E t);

extern void Tag_lookup_free(T *t);

#undef T
#ifdef __cplusplus
}
#endif
#endif /* __TAG_LOOKUP_INCLUDED__ */
