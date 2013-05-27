#ifndef __DOCUMENT_TREE_INCLUDED__
#define __DOCUMENT_TREE_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include "html_parser_document_node.h"

#define T Doc_tree_T
typedef struct T *T;

extern T Doc_tree_tree(void);
extern int Doc_tree_size(T tr);
extern Node_T *Doc_tree_root(T *tr);

extern void Doc_tree_insert(T tr, Node_T *n);
extern int Doc_tree_end_tag(T tr, const char *tag_name);

/* clone matched node including all children */
extern Node_T Doc_tree_graft(Node_T *m);

/* remove all descendents of parent with match */
extern T Doc_tree_trim(T tr, Node_T *match);

extern char *Doc_tree_print(T tr);
extern char *Doc_tree_print_context(T tr);
extern void Doc_tree_free(T *tr);

#undef T

#ifdef __cplusplus
}
#endif

#endif /* __DOCUMENT_TREE_INCLUDED__ */
