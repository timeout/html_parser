#ifndef __DOCUMENT_SEARCH_INCLUDED__
#define __DOCUMENT_SEARCH_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include "html_parser_attribute_rep.h"
#include "html_parser_document_tree.h"

#define T Match_T
typedef struct T *T;

extern int Doc_search_result_size(T r);

extern T Doc_search_tag(Doc_tree_T *tr, const char *tname, 
		const char *aname, const char *aval);
extern T Doc_search_sub_tree(Doc_tree_T *tr, const char *tname,
		const char *aname, const char *aval);
extern T Doc_search_content(Doc_tree_T *tr, const char *str);

extern const char *Doc_search_result_print(T *r);
extern void Doc_search_free(T *r);

#undef T

#ifdef __cplusplus
}
#endif

#endif /* __DOCUMENT_SEARCH_INCLUDED__ */
