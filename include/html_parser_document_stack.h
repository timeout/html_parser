#ifndef __document_stack_included__
#define __document_stack_included__

#include "html_parser_document_node.h"

#ifdef __cplusplus
extern "C" {
#endif

#define T Stack_T
typedef struct T *T;

extern T Stack_stack(void);
extern int Stack_empty(T stk);
extern void Stack_push(T stk, Node_T *n);
extern Node_T *Stack_peek(T stk);
extern int Stack_find(T stk, const char *tag_name);
extern Node_T *Stack_pop(T stk);
extern void Stack_free(T *Stk);

#undef T
#ifdef __cplusplus
}
#endif

#endif /* __document_stack_included__ */
