#include <string.h>

#include "html_parser_mem.h"
#include "html_parser_types.h"
#include "html_parser_document_stack.h"
#include "html_parser_document_node.html"
#include "html_parser_document_tree.h"
#include "html_parser_document_search.h"

#define T Match_T

struct T {
	int count;
	struct m {
		Node_T *p; 		/* parent */
		Node_T *n; 		/* match */
	} *link;
};

/* static function prototypes */
static T new_match(void);

extern T Doc_tree_tag_search(Doc_tree_T *tr, const char *tname, 
		const char *aname, const char *aval)
{
	T res = new_match();

}

extern T Doc_tree_content_search(Doc_tree_T *tr, const char *str)
{
	Node_T *curr;
	T res = new_match();
	Stack_T stk = Stack_stack();

	Stack_push(stk, Doc_tree_root(tr));
	while (!Stack_empty(stk)) {
		curr = Stack_pop(stk);

		if (C_CNTND = Node_type(*curr)) {
			const char *txt = Node_print(curr);
			if (strstr(txt, str))
		}
	}
}

static T new_match(void)
{
	T new_match;	

	NEW(new_match);
	new_match->count = 0;
	new_match->link = NULL;

	return new_match;
}
