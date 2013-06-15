#ifndef __document_stack_included__
#define __document_stack_included__

#ifdef __cplusplus
extern "C" {
#endif

#define T Stack_T
typedef struct T *T;

extern T Stack_stack(void);
extern int Stack_empty(T stk);
extern void Stack_push(T stk, void *n);
extern void *Stack_peek(T stk);
// extern int Stack_find(T stk, const char *tag_name);
extern void *Stack_pop(T stk);
extern void Stack_free(T *stk);

#undef T
#ifdef __cplusplus
}
#endif

#endif /* __document_stack_included__ */
