#include <string.h>
#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_atom.h"
#include "html_parser_fmt.h"
#include "html_parser_document_node.h"
#include "html_parser_document_tree.h"
#include "html_parser_tag_lookup.h"
#include "html_parser_tester.h"

#define T Test_T

#define NODE_SIZE 6

/* test data */
Lookup_T tbl;

/* static function prototypes */
static Node_T *init_nodes(void);

/* test functions */
static int Test_doc_tree_tree(T t, void *, const void *);
static int Test_doc_tree_insert(T t, void *, const void *);
static int Test_doc_tree_end_tag(T t, void *, const void *);
static int Test_doc_tree_size(T t, void *, const void *);
static int Test_doc_tree_print(T t, void *, const void *);
static int Test_doc_tree_print_content(T t, void *, const void *);
static int Test_doc_tree_free(T t, void *, const void *);

int main(int argc, const char *argv[])
{
	T suite;
	Doc_tree_T d;
	Node_T *nodes;
	const char *ct = Atom_str("p");
	
	Fmt_fprint(stderr, "==> Starting %s <==\n", __FILE__);

	tbl = Tag_lookup_init();
	nodes = init_nodes();
	suite = Test_init();

	Test_add(suite, Test_doc_tree_tree, (void *) &d, NULL);
	Test_add(suite, Test_doc_tree_insert, PREV_INPUT(suite),
			(const void *) nodes);
	Test_add(suite, Test_doc_tree_end_tag, PREV_INPUT(suite),
			(const void *) ct);
	Test_add(suite, Test_doc_tree_size, PREV_INPUT(suite),
			(const void *) ct);
	Test_add(suite, Test_doc_tree_print, PREV_INPUT(suite), NULL);
	Test_add(suite, Test_doc_tree_print_content, PREV_INPUT(suite), NULL);
	Test_add(suite, Test_doc_tree_free, PREV_INPUT(suite), NULL);

	Test_all_run(suite);
	Test_print_results(suite);

	Test_free(&suite);
	Tag_lookup_free(&tbl);

	return 0;
}

static int Test_doc_tree_tree(T t, void *s, const void *chk)
{
	Doc_tree_T *d = (Doc_tree_T *) s;
	*d = Doc_tree_tree();

	assert(d && *d);

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_doc_tree_insert(T t, void *s, const void *chk)
{
	Doc_tree_T *doc = (Doc_tree_T *) s;
	Node_T *n = (Node_T *) chk;

	Doc_tree_insert(*doc, &n[0]); 	/* html */
	Doc_tree_insert(*doc, &n[1]); 	/* body */
	Doc_tree_insert(*doc, &n[2]); 	/* p */
	Doc_tree_insert(*doc, &n[4]); 	/* "Harlem Shuffle" */
	Doc_tree_insert(*doc, &n[3]); 	/* em */
	Doc_tree_insert(*doc, &n[5]); 	/* "Hold Back" */

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_doc_tree_end_tag(T t, void *s, const void *chk)
{
	Doc_tree_T *doc = (Doc_tree_T *) s;
	const char *ct = (const char *) chk; 	/* p */

	assert(doc && *doc);
	assert(ct && *ct);

	Doc_tree_end_tag(*doc, ct);

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_doc_tree_size(T t, void *s, const void *chk)
{
	Doc_tree_T *doc = (Doc_tree_T *) s;
	
	TEST_FUNC_NAME(t);
	TEST_FUNC_OUT(t, Fmt_string("%d", Doc_tree_size(*doc)));

	return TEST_SUCCESS;
}

static int Test_doc_tree_print(T t, void *s, const void *chk)
{
	Doc_tree_T *doc = (Doc_tree_T *) s;

	TEST_FUNC_NAME(t);
	TEST_FUNC_OUT(t, Doc_tree_print(*doc));

	return TEST_SUCCESS;
}

static int Test_doc_tree_print_content(T t, void *s, const void *chk)
{
	Doc_tree_T *doc = (Doc_tree_T *) s;

	TEST_FUNC_NAME(t);
	TEST_FUNC_OUT(t, Doc_tree_print_context(*doc));

	return TEST_FAIL;
}

static int Test_doc_tree_free(T t, void *s, const void *chk)
{
	Doc_tree_T *doc = (Doc_tree_T *) s;

	Doc_tree_free(doc);

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
	n[4] = Node_new_content(Fmt_string("Harlem Shuffle"));
	n[5] = Node_new_content(Fmt_string("Hold Back"));

	return n;
}
