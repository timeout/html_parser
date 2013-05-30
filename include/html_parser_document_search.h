#ifndef __DOCUMENT_SEARCH_INCLUDED__
#define __DOCUMENT_SEARCH_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include "html_parser_attribute_rep.h"
#include "html_parser_document_tree.h"

#define T Match_T
typedef struct T *T;

extern int Doc_tree_results(T r);
extern T Doc_tree_tag_search(Doc_tree_T *tr, const char *tname, 
		const char *aname, const char *aval);
extern T Doc_tree_content_search(Doc_tree_T *tr, const char *str);

#undef T

#ifdef __cplusplus
}
#endif

#endif /* __DOCUMENT_SEARCH_INCLUDED__ */
