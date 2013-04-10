#ifndef __DOCUMENT_TREE_INCLUDED__
#define __DOCUMENT_TREE_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include "html_parser_document_node.h"

#define T Doc_tree_T
typedef struct T *T;

extern T Doc_tree_tree(void);
extern void Doc_tree_insert(T tree, Node_T *n);
extern int Doc_tree_end_tag(T tree, const char *tag_name);

extern char *Doc_tree_print(T tree);
extern void Doc_tree_free(T *tree);

#undef T

#ifdef __cplusplus
}
#endif

#endif /* __DOCUMENT_TREE_INCLUDED__ */
