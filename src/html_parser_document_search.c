#include <string.h>

#include "html_parser_mem.h"
#include "html_parser_types.h"
#include "html_parser_document_stack.h"
#include "html_parser_document_node.h"
#include "html_parser_document_tree.h"
#include "html_parser_document_search.h"

#define T Match_T

struct T {
	int count;
	struct m {
		Node_T *p; 		/* parent */
		Node_T *n; 		/* match */
		struct m *link;
	} *el;
};

/* static function prototypes */
static T new_match(void);
static void tag_match(T *resp, Node_T *curr, Node_T *target, const char *tname,
		const char *aname, const char *aval);
static void cont_match(T *resp, Node_T *curr, Node_T *target, const char *str);

extern int Doc_tree_results(T r)
{
	return r->count;
}

extern T Doc_tree_tag_search(Doc_tree_T *tr, const char *tname, 
		const char *aname, const char *aval)
{
	Node_T *curr, *tmp; 		/* tmp is node of interest */
	T res = new_match();
	Stack_T stk = Stack_stack();

	Stack_push(stk, Doc_tree_root(tr));
	while (!Stack_empty(stk)) {
		curr = Stack_pop(stk);

		/* sibling */
		if (Node_has_sibling(curr)) {
			tmp = Node_sibling(curr);
			if (C_TAGND == Node_type(*tmp))  /* check type here */
				tag_match(&res, curr, tmp, tname, aname, aval);
			Stack_push(stk, tmp);
		}

		/* child */
		if (Node_has_child(curr)) {
			tmp = Node_child(curr);
			if (C_TAGND == Node_type(*tmp)) 
				tag_match(&res, curr, tmp, tname, aname, aval);
			Stack_push(stk, tmp);
		}
	}

	return res;
}

extern T Doc_tree_content_search(Doc_tree_T *tr, const char *str)
{
	Node_T *curr, *tmp; 		/* tmp is node of interest */
	T res = new_match();
	Stack_T stk = Stack_stack();

	Stack_push(stk, Doc_tree_root(tr));
	while (!Stack_empty(stk)) {
		curr = Stack_pop(stk);

		/* sibling */
		if (Node_has_sibling(curr)) {
			tmp = Node_sibling(curr);
			if (C_CNTND == Node_type(*tmp))  /* check type here */
				cont_match(&res, curr, tmp, str);
			Stack_push(stk, tmp);
		}

		/* child */
		if (Node_has_child(curr)) {
			tmp = Node_child(curr);
			if (C_CNTND == Node_type(*tmp)) 
				cont_match(&res, curr, tmp, str);
			Stack_push(stk, tmp);
		}
	}

	return res;
}

static T new_match(void)
{
	T new_match;	

	NEW(new_match);
	new_match->count = 0;
	new_match->el = NULL;

	return new_match;
}

static void cont_match(T *resp, Node_T *curr, Node_T *target, const char *str)
{
	char *txt = (char *) Node_print(target);

	/* search */
	if (strstr(txt, str)) {
		/* create match */
		struct m *match;
		NEW(match);
		match->p = curr;
		match->n = target;
		match->link = NULL;

		/* add match to resp */
		if (Doc_tree_results(*resp)) {
			struct m* tmp = (*resp)->el;
			while (tmp->link != NULL)
				tmp = tmp->link;
			tmp->link = match;
		} else {
			(*resp)->el = match;
		}
		(*resp)->count++;
	}

	FREE(txt);
}

/* this is nasty */
static void tag_match(T *resp, Node_T *curr, Node_T *target, const char *tname,
		const char *aname, const char *aval)
{
	Attr_list_T *alist;
	struct m *match;

	/* search: as soon as there's a match add the node and return */
	if (tname && (strcmp(tname, Node_name(*target)) == 0))
		goto matched;

	alist = Node_attr_list(*target);
	if (alist && Attr_list_search(alist, aname, aval))
		goto matched;

	return; 	/* no match */

matched:
	/* create match */
	NEW(match);
	match->p = curr;
	match->n = target;
	match->link = NULL;

	/* add match to resp */
	if (Doc_tree_results(*resp)) {
		struct m* tmp = (*resp)->el;
		while (tmp->link != NULL)
			tmp = tmp->link;
		tmp->link = match;
	} else {
		(*resp)->el = match;
	}
	(*resp)->count++;
}

