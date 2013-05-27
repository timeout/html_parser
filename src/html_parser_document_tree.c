#include <stdlib.h>
#include <string.h>
#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_fmt.h"
#include "html_parser_text.h"
#include "html_parser_types.h"
#include "html_parser_document_node.h"
#include "html_parser_document_stack.h"
#include "html_parser_document_tree.h"

#define T Doc_tree_T

/* typed definition */
struct T {
	Node_T root;
	Stack_T st;
};

/* static function prototypes */
static void map(T tree, void apply(Node_T **, void *), void *);
static void dt_print(Node_T **, void *);
static void dt_print_context(Node_T **, void *);
static void dt_node_free(Node_T **, void *);
static void dt_size(Node_T **, void *);

T Doc_tree_tree(void)
{
	T new_tree;

	NEW(new_tree);

	new_tree->root = Node_new_tag(T_OPTNL, "/", NULL);
	new_tree->st = Stack_stack();

	assert(new_tree);

	return new_tree;
}

void Doc_tree_insert(T tree, Node_T *n)
{
	Node_T *curr = NULL;

	assert(tree);
	assert(n && *n);

	if (Stack_empty(tree->st)) {
		curr = &tree->root;
	}
	else {
		curr = Stack_peek(tree->st);
	}

	/* T_OPTNL */
	if ((C_TAGND == Node_type(*n))
			&& (T_OPTNL == Node_tag_type(*n)) 
			&& (T_OPTNL == Node_tag_type(*curr)))
		Doc_tree_end_tag(tree, Node_name(*curr));

	/* insert */
	if (Node_has_child(curr)) {
		Node_T *tmp = Node_last_sibling(Node_child(curr));
		Node_add_sibling(tmp, n);

		if ((C_TAGND == Node_type(*n)) && 
				(T_VOID != Node_tag_type(*n)))
			Stack_push(tree->st, n);
	} else {
		Node_add_child(curr, n);

		if ((C_TAGND == Node_type(*n))
				&& (T_VOID != Node_tag_type(*n)))
			Stack_push(tree->st, n);
	}

}


int Doc_tree_end_tag(T tree, const char *tag_name)
{
	int n;

	assert(tree);
	assert(tag_name && *tag_name);

	n = Stack_find(tree->st, tag_name);

	if (n) {
		for (int i = 0; i < n; i++)
			Stack_pop(tree->st);
	}

	return n;
}

/* Given a node, in particular a node matching a search query, clone the
 * node and all the node's children */
Node_T Doc_tree_graft(Node_T *match)
{
	Node_T cl;
	Node_T *curr, *clp;
	Stack_T stk = Stack_stack();
	Stack_T cl_stk = Stack_stack();

	assert(match && *match);

	cl = Node_clone(*match);

	Stack_push(stk, match);
	Stack_push(cl_stk, &cl);
	while (!Stack_empty(stk)) {
		curr = Stack_pop(stk);
		clp = Stack_pop(cl_stk);

		if (Node_has_sibling(curr)) {
			Node_T *tmp = Node_sibling(curr);
			Node_T ns = Node_clone(*tmp);
			Node_add_sibling(clp, &ns);
			Stack_push(stk, tmp);
			Stack_push(cl_stk, &ns);
		}

		if (Node_has_child(curr)) {
			Node_T *tmp = Node_child(curr);
			Node_T nc = Node_clone(*tmp);
			Node_add_child(clp, &nc);
			Stack_push(stk, tmp);
			Stack_push(cl_stk, &nc);
		}
	}

	return cl;
}

int Doc_tree_size(T tr)
{
	int s;

	map(tr, dt_size, &s);

	return s;
}

Node_T *Doc_tree_root(T *tr)
{
	return &(*tr)->root;
}

char *Doc_tree_print(T tree)
{
	Text_T p = Text_box("", 0);

	Fmt_register('T', Text_fmt);

	map(tree, dt_print, (void *) &p);

	return Fmt_string("%T\n", &p);
}

extern char *Doc_tree_print_context(T tr)
{
	Text_T p = Text_box("", 0);

	Fmt_register('T', Text_fmt);

	map(tr, dt_print_context, (void *) &p);

	return Fmt_string("%T\n", &p);
}

void Doc_tree_free(T *tree)
{
	assert(tree && *tree);

	/* Nodes */
	map(*tree, dt_node_free, NULL);

	/* doc stack */
	Stack_free(&(*tree)->st);

	/* tree */
	FREE(*tree);
}

static void map(T tree, void apply(Node_T **n, void *cl), void *cl)
{
	Node_T *curr = &tree->root;
	Stack_T stk = Stack_stack();

	assert(apply);

	Stack_push(stk, curr);
	while (!Stack_empty(stk)) {
		curr = Stack_pop(stk);

		if (Node_has_sibling(curr)) {
			Stack_push(stk, Node_sibling(curr));
		}
		if (Node_has_child(curr)) {
			Stack_push(stk, Node_child(curr));
		}

		apply(&curr, cl);
	}
}

static void dt_size(Node_T **n, void *cl)
{
	int *s = (int *) cl;

	*s += 1;
}

static void dt_print(Node_T **n, void *cl)
{
	Text_T *p = (Text_T *) cl;

	assert(n && **n);

	*p = Text_cat(*p, Text_put(Node_print(*n)));
	*p = Text_cat(*p, Text_put("\n"));
}

static void dt_print_context(Node_T **n, void *cl)
{
	Text_T *p = (Text_T *) cl;

	assert(n && *n);

	if (C_CNTND == Node_type(**n)) {
		*p = Text_cat(*p, Text_put(Node_print(*n)));
		*p = Text_cat(*p, Text_put("\n"));
	}
}

static void dt_node_free(Node_T **n, void *cl)
{
	assert(n && **n);

	Node_free(*n);
}
