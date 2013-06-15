#ifndef __DOCUMENT_TREE_INCLUDED__
#define __DOCUMENT_TREE_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include "html_parser_types.h"
#include "html_parser_document_node.h"

#define T Doc_tree_T
typedef struct T *T;

#define INDENT_SIZE 2

extern Node_T *Doc_tree_curr(T tr);
extern T Doc_tree_insert(T tr, Node_T *n);
extern T Doc_tree_end(T tr, Tag_E tf, const char *tend);

extern void Doc_tree_print(T tr);

extern void Doc_tree_free(T *tr);

#undef T

#ifdef __cplusplus
}
#endif

#endif /* __DOCUMENT_TREE_INCLUDED__ */
