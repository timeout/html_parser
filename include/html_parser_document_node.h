#ifndef __DOCUMENT_NODE_INCLUDED__
#define __DOCUMENT_NODE_INCLUDED__

#include "html_parser_attribute_list.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "html_parser_types.h"

#define T Node_T
typedef struct T *T;

extern T Node_new_tag(Tag_E tf, const char *name, Attr_list_T *attrs);
extern T Node_new_content(const char *text);
extern T Node_clone(T n);

extern const char *Node_name(T n);
extern Chunk_E Node_type(T n);
extern Tag_E Node_tag_type(T n);
extern Attr_list_T *Node_attr_list(T n);

extern int Node_has_child(T *n);
extern int Node_has_sibling(T *n);
extern int Node_has_back(T *n);
extern int Node_has_parent(T *n);
extern int Node_is_first_child(T *n);
extern T *Node_parent(T *n);
extern T *Node_back(T *n);
extern T *Node_child(T *n);
extern T *Node_sibling(T *n);

extern T *Node_add_child(T *parent, T *child);
extern T *Node_add_sibling(T *first, T *second);
extern T *Node_add_parent(T *parent, T *child);

extern const char *Node_print(T *n);
extern void Node_free(T *n);

#undef T

#ifdef __cplusplus
}
#endif
#endif /* __DOCUMENT_NODE_INCLUDED__ */
