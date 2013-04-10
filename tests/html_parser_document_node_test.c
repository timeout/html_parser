#include <string.h>
#include <stdio.h>
#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_atom.h"
#include "html_parser_fmt.h"
#include "html_parser_text.h"
#include "html_parser_attribute_list.h"
#include "html_parser_document_node.h"
#include "html_parser_tester.h"

#define T Test_T

/* test data */

/* test functions */
static int Test_node_new_tag(T t, void *, const void *);
static int Test_node_new_content(T t, void *, const void *);
static int Test_node_type_tag(T t, void *, const void *);
static int Test_node_type_content(T t, void *, const void *);
static int Test_node_has_child(T t, void *, const void *);
static int Test_node_has_sibling(T t, void *, const void *);
static int Test_node_child(T t, void *, const void *);
static int Test_node_sibling(T t, void *, const void *);
static int Test_node_last_sibling(T t, void *, const void *);
static int Test_node_add_child(T t, void *, const void *);
static int Test_node_add_sibling(T t, void *, const void *);
static int Test_node_print_tag(T t, void *, const void *);
static int Test_node_print_content(T t, void *, const void *);
static int Test_node_free(T t, void *, const void *);


int main(int argc, const char *argv[])
{
	T suite;
	Node_T tag_node = NULL, content_node = NULL, sibling_node = NULL;
	const char *name = "h1";
	const char *txt =  "The author of this book, it's" 
			" setting and characters are entirely fictitious."
			" There is no such place as the state of Victoria."
			" The Australian Labour Party exists only in the"
			" imagination of its members.";
	
	Fmt_fprint(stderr, "==> Starting %s <==\n", __FILE__);

	suite = Test_init();

	Test_add(suite, Test_node_new_tag, (void *) &tag_node, 
			(const void *) name);
	Test_add(suite, Test_node_new_content, (void *) &content_node, 
			(const void *) txt);
	Test_add(suite, Test_node_type_tag, NULL, (const void *) &tag_node);
	Test_add(suite, Test_node_type_content, NULL, (const void *) &content_node);
	Test_add(suite, Test_node_has_child, NULL, (const void *) &tag_node);
	Test_add(suite, Test_node_has_sibling, NULL, (void *) &tag_node);
	Test_add(suite, Test_node_add_child, (void *) &tag_node,
			(const void *) &content_node);
	Test_add(suite, Test_node_add_sibling, (void *) &content_node, NULL);
	Test_add(suite, Test_node_child, NULL, (const void *) &tag_node);
	Test_add(suite, Test_node_sibling, (void *) &sibling_node,
			(const void *) &content_node);
	Test_add(suite, Test_node_last_sibling, NULL, 
			(const void *) &content_node);
	Test_add(suite, Test_node_print_tag, NULL, (const void *) &tag_node);
	Test_add(suite, Test_node_print_content, NULL, (void *) &content_node);
	Test_add(suite, Test_node_free, (void *) &tag_node, NULL);

	Test_all_run(suite);
	Test_print_results(suite);

	Test_free(&suite);

	return 0;
}

static int Test_node_new_tag(T t, void *s, const void *chk)
{
	Node_T *tn = (Node_T *) s;
	const char *n = (const char *) chk;

	assert(n && *n);

	*tn = Node_new_tag(n, NULL);
	assert(*tn);

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_node_new_content(T t, void *s, const void *chk)
{
	Node_T *cn = (Node_T *) s;
	const char *c = (const char *) chk;

	assert(c && *c);

	*cn = Node_new_content(c);
	assert(*cn);

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_node_type_tag(T t, void *s, const void *chk)
{
	Node_T *tn = (Node_T *) chk;

	assert(TAG_NODE == Node_type(*tn));

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_node_type_content(T t, void *s, const void *chk)
{
	Node_T *cn = (Node_T *) chk;

	assert(CONTENT_NODE == Node_type(*cn));

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}
static int Test_node_has_child(T t, void *s, const void *chk)
{
	Node_T *tn = (Node_T *) chk;

	assert(Node_has_child(tn) == 0);
	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_node_has_sibling(T t, void *s, const void *chk)
{
	Node_T *cn = (Node_T *) chk;

	assert(Node_has_sibling(cn) == 0);

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_node_child(T t, void *s, const void *chk)
{
	Node_T *tn = (Node_T *) chk;
	Node_T *cn = Node_child(tn);

	assert(*cn);

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_node_sibling(T t, void *s, const void *chk)
{
	Node_T *sn = (Node_T *) s;
	Node_T *cn = (Node_T *) chk;

	sn = Node_sibling(cn);

	assert(*sn);

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_node_last_sibling(T t, void *s, const void *chk)
{
	Node_T older, younger, youngest, *sn;
	Node_T *cn = (Node_T *) chk; 	/* cn has a sibling <em> */

	/* create and add some siblings */
	older = Node_new_tag(Atom_str("strong"), NULL);
	younger = Node_new_tag(Atom_str("i"), NULL);
	youngest = Node_new_tag(Atom_str("b"), NULL);

	Node_add_sibling(&older, &younger);
	Node_add_sibling(&younger, &youngest);
	Node_add_sibling(&youngest, cn);

	sn = Node_last_sibling(&older);

	assert(sn == Node_sibling(cn));

	char *msg = (char *) Node_print(sn);

	TEST_FUNC_NAME(t);
	TEST_FUNC_OUT(t, Fmt_string("%s", msg));

	/* destroy some siblings */
	Node_free(&older);
	Node_free(&younger);
	Node_free(&youngest);

	return TEST_SUCCESS;
}

static int Test_node_add_child(T t, void *s, const void *chk)
{
	Node_T *tn = (Node_T *) s;
	Node_T *cn = (Node_T *) chk;

	Node_add_child(tn, cn);

	TEST_FUNC_NAME(t);

	return Node_has_child(tn);
}

static int Test_node_add_sibling(T t, void *s, const void *chk)
{
	Node_T *cn = (Node_T *) s;
	Node_T em = Node_new_tag("em", NULL);

	Node_add_sibling(cn, &em);

	TEST_FUNC_NAME(t);

	return Node_has_sibling(cn);
}

static int Test_node_print_tag(T t, void *s, const void *chk)
{
	Node_T *tn = (Node_T *) chk;
	char *msg = (char *) Node_print(tn);

	TEST_FUNC_NAME(t);
	TEST_FUNC_OUT(t, Fmt_string("%s", msg));

	return TEST_SUCCESS;
}

static int Test_node_print_content(T t, void *s, const void *chk)
{
	Node_T *cn = (Node_T *) chk;
	char *msg = (char *) Node_print(cn);

	TEST_FUNC_NAME(t);
	TEST_FUNC_OUT(t, Fmt_string("%s", msg));

	return TEST_SUCCESS;
}

static int Test_node_free(T t, void *s, const void *chk)
{
	Node_T *cn, *sn;
	Node_T *top = (Node_T *) s; 	/* tag_node */

	assert(top && *top);

	cn = Node_child(top);
	sn = Node_sibling(cn);

	assert(cn && *cn);
	assert(sn && *sn);

	Node_free(sn);
	Node_free(cn);

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}
