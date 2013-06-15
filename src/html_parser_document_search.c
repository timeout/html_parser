#include <string.h>

#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_types.h"
#include "html_parser_text.h"
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
static T tag_search(Doc_tree_T *tr, const char *tname,
		const char *aname, const char *aval, int sub_on);
static int tag_match(T *resp, Node_T *curr, Node_T *target, const char *tname,
		const char *aname, const char *aval);
static void cont_match(T *resp, Node_T *curr, Node_T *target, const char *str);
static void map(T *resp, void apply(struct m **, void *cl), void *cl);
static void results_print(struct m**, void *cl);

int Doc_search_result_size(T r)
{
	assert(r);

	return r->count;
}

/* wrappers */
T Doc_search_tag(Doc_tree_T *tr, const char *tname, 
		const char *aname, const char *aval)
{
	return tag_search(tr, tname, aname, aval, 0);
}

T Doc_search_sub_tree(Doc_tree_T *tr, const char *tname,
		const char *aname, const char *aval)
{
	return tag_search(tr, tname, aname, aval, 1);
}

T Doc_search_content(Doc_tree_T *tr, const char *str)
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

void Doc_search_free(T *r)
{
	struct m *tmp = (*r)->el;

	while (tmp != NULL) {
		(*r)->el = tmp->link;
		FREE(tmp);
	}

	FREE(*r);
}

static T new_match(void)
{
	T new_match;	

	NEW(new_match);
	new_match->count = 0;
	new_match->el = NULL;

	return new_match;
}

static T tag_search(Doc_tree_T *tr, const char *tname,
		const char *aname, const char *aval, int sub_on)
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
			if (C_TAGND == Node_type(*tmp)) {  /* check type here */
				tag_match(&res, curr, tmp, tname, aname, aval);
				Stack_push(stk, tmp);
			}
		}

		/* child */
		if (Node_has_child(curr)) {
			tmp = Node_child(curr);
			if (C_TAGND == Node_type(*tmp)) {
				if (tag_match(&res, curr, tmp, tname, aname, aval) &&
						sub_on) {
					continue;
				}
				Stack_push(stk, tmp);
			}
		}
	}

	return res;

}

const char *Doc_search_result_print(T *r)
{
	Text_T output = Text_box("", 0);

	Fmt_register('T', Text_fmt);

	map(r, results_print, (void *) &output);

	return Fmt_string("%T\n", &output);
}

/* this is nasty */
static int tag_match(T *resp, Node_T *curr, Node_T *target, const char *tname,
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

	return 0; 	/* no match */

matched:
	/* create match */
	NEW(match);
	match->p = curr;
	match->n = target;
	match->link = NULL;

	/* add match to resp */
	if (Doc_search_result_size(*resp)) {
		struct m* tmp = (*resp)->el;
		while (tmp->link != NULL)
			tmp = tmp->link;
		tmp->link = match;
	} else {
		(*resp)->el = match;
	}
	(*resp)->count++;

	return 1;
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
		if (Doc_search_result_size(*resp)) {
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

static void map(T *resp, void apply(struct m **, void *cl), void *cl)
{
	struct m *tmp; 

	assert(resp && *resp);
	assert(apply);

	tmp = (*resp)->el;

	while (tmp != NULL) {
		apply(&tmp, cl);
		tmp = tmp->link;
	}
}

static void results_print(struct m **rpp, void *cl)
{
	Text_T *output = (Text_T *) cl;
	Node_T *p = (*rpp)->p;
	Node_T *n = (*rpp)->n;

	*output = Text_cat(*output, Text_put("predecessor:\n"));
	*output = Text_cat(*output, Text_put(Node_print(p)));
	*output = Text_cat(*output, Text_put("\n"));
	*output = Text_cat(*output, Text_put("match:\n"));
	*output = Text_cat(*output, Text_put(Node_print(n)));
	*output = Text_cat(*output, Text_put("\n"));
}
