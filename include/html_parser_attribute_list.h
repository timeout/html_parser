#ifndef __ATTRIBUTE_LIST_INCLUDED__
#define __ATTRIBUTE_LIST_INCLUDED__

#include "html_parser_attribute_rep.h"

#ifdef __cplusplus
extern "C" {
#endif

#define T Attr_list_T
typedef struct T *T;

extern T Attr_list_list(void);
extern int Attr_list_length(T attrs);
extern T Attr_list_enqueue(T attrs, Attr_rep_T attr);
extern Attr_rep_T Attr_list_dequeue(T head);
extern void Attr_list_free(T *attrs);
extern const char *Attr_list_print(T attrs);

#undef T

#ifdef __cplusplus
}
#endif
#endif /* __ATTRIBUTE_LIST_INCLUDED__ */
