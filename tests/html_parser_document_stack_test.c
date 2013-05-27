#include <string.h>
#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_atom.h"
#include "html_parser_fmt.h"
#include "html_parser_document_node.h"
#include "html_parser_document_stack.h"
#include "html_parser_tag_lookup.h"
#include "html_parser_tester.h"

#define T Test_T

#define NODE_SIZE 4

/* test data */
Lookup_T tbl;

/* static function prototypes */
static Node_T *init_nodes(void);

/* test functions */
static int Test_stack_stack(T t, void *, const void *);
static int Test_stack_empty(T t, void *, const void *);
static int Test_stack_push(T t, void *, const void *);
static int Test_stack_peek(T t, void *, const void *);
static int Test_stack_find(T t, void *, const void *);
static int Test_stack_pop(T t, void *, const void *);
static int Test_stack_free(T t, void *, const void *);

int main(int argc, const char *argv[])
{
	T suite;

	Stack_T s;
	Node_T *nodes;
	const char *st = Atom_str("p");

	Fmt_fprint(stderr, "==> Starting %s <==\n", __FILE__);

	tbl = Tag_lookup_init();
	nodes = init_nodes();
	suite = Test_init();

	Test_add(suite, Test_stack_stack, (void *) &s, NULL);
	Test_add(suite, Test_stack_empty, PREV_INPUT(suite), NULL);
	Test_add(suite, Test_stack_push, PREV_INPUT(suite),
			(const void *) nodes);
	Test_add(suite, Test_stack_peek, PREV_INPUT(suite),
			(const void *) nodes);
	Test_add(suite, Test_stack_find, PREV_INPUT(suite),
			(const void *) st);
	Test_add(suite, Test_stack_pop, PREV_INPUT(suite),
			(const void *) nodes);
	Test_add(suite, Test_stack_free, PREV_INPUT(suite), NULL);

	Test_all_run(suite);
	Test_print_results(suite);

	Test_free(&suite);
	Tag_lookup_free(&tbl);

	return 0;
}

static int Test_stack_stack(T t, void *s, const void *chk)
{
	Stack_T *stk = (Stack_T *) s;

	*stk = Stack_stack();
	assert(*stk);

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_stack_empty(T t, void *s, const void *chk)
{
	Stack_T *stk = (Stack_T *) s;

	assert(*stk);
	assert(Stack_empty(*stk) == 1);

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_stack_push(T t, void *s, const void *chk)
{
	Stack_T *stk = (Stack_T *) s;
	Node_T *n = (Node_T *) chk;

	assert(*stk);
	assert(n && *n);

	for (int i = 0; i < NODE_SIZE; i++) {
		Stack_push(*stk, &n[i]);
		assert(Stack_empty(*stk) == 0);
	}

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_stack_peek(T t, void *s, const void *chk)
{
	Stack_T *stk = (Stack_T *) s;
	Node_T *n = (Node_T *) chk;

	assert(*stk);
	assert(n && *n);

	assert(&n[NODE_SIZE -1] == Stack_peek(*stk));

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_stack_find(T t, void *s, const void *chk)
{
	Stack_T *stk = (Stack_T *) s;
	const char *st = (const char *) chk, *bt = Atom_str("bogie");

	assert(2 == Stack_find(*stk, st));
	assert(0 == Stack_find(*stk, bt));

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_stack_pop(T t, void *s, const void *chk)
{
	Stack_T *stk = (Stack_T *) s;
	Node_T *tmp, *n = (Node_T *) chk;

	assert(*stk);
	assert(n && *n);

	tmp = Stack_pop(*stk);
	assert(tmp == &n[NODE_SIZE -1]);
	Node_free(tmp);

	tmp = Stack_pop(*stk);
	assert(tmp == &n[NODE_SIZE -2]);
	Node_free(tmp);

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_stack_free(T t, void *s, const void *chk)
{
	Stack_T *stk = (Stack_T *) s;

	assert(*stk);

	Stack_free(stk);

	assert(*stk == NULL);

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static Node_T *init_nodes(void)
{
	Node_T *n = ALLOC(NODE_SIZE * sizeof(*n));

	n[0] = Node_new_tag(Tag_lookup_tag(&tbl, "html"), Atom_str("html"), NULL);
	n[1] = Node_new_tag(Tag_lookup_tag(&tbl, "body"), Atom_str("body"), NULL);
	n[2] = Node_new_tag(Tag_lookup_tag(&tbl, "p"), Atom_str("p"), NULL);
	n[3] = Node_new_tag(Tag_lookup_tag(&tbl, "em"), Atom_str("em"), NULL);

	return n;
}

