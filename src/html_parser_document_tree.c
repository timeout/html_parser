#include <stdlib.h>
#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_fmt.h"
#include "html_parser_text.h"
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
static void dt_node_free(Node_T **, void *);


T Doc_tree_tree(void)
{
	T new_tree;

	NEW(new_tree);

	new_tree->root = Node_new_tag("/", NULL);
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

	if (Node_has_child(curr)) {
		Node_T *tmp = Node_last_sibling(Node_child(curr));
		Node_add_sibling(tmp, n);

		if (TAG_NODE == Node_type(*n))
			Stack_push(tree->st, n);
	} else {
		Node_add_child(curr, n);

		if (TAG_NODE == Node_type(*n))
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

char *Doc_tree_print(T tree)
{
	Text_T p = Text_box("", 0);

	Fmt_register('T', Text_fmt);

	map(tree, dt_print, (void *) &p);

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

static void dt_print(Node_T **n, void *cl)
{
	Text_T *p = (Text_T *) cl;

	assert(n && **n);

	*p = Text_cat(*p, Text_put(Node_print(*n)));
	*p = Text_cat(*p, Text_put("\n"));
}

static void dt_node_free(Node_T **n, void *cl)
{
	assert(n && **n);

	Node_free(*n);
}
