#include <string.h>
#include <stdio.h>
#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_atom.h"
#include "html_parser_fmt.h"
#include "html_parser_text.h"
#include "html_parser_attribute_rep.h"
#include "html_parser_attribute_list.h"
#include "html_parser_document_node.h"
#include "html_parser_tag_lookup.h"
#include "html_parser_tester.h"

#define T Test_T

/* test data */
Lookup_T tbl;

/* test functions */
static int Test_node_new_tag(T t, void *, const void *);
static int Test_node_new_content(T t, void *, const void *);
static int Test_node_type_tag(T t, void *, const void *);
static int Test_node_type_content(T t, void *, const void *);
static int Test_node_has_child(T t, void *, const void *);
static int Test_node_has_sibling(T t, void *, const void *);
static int Test_node_child(T t, void *, const void *);
static int Test_node_sibling(T t, void *, const void *);
static int Test_node_add_child(T t, void *, const void *);
static int Test_node_add_sibling(T t, void *, const void *);
static int Test_node_parent(T t, void *, const void *);
static int Test_node_back(T t, void *, const void *);
static int Test_node_add_parent(T t, void *, const void *);
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

	tbl = Tag_lookup_init();

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
	Test_add(suite, Test_node_parent, NULL, NULL);
	Test_add(suite, Test_node_back, NULL, NULL);
	Test_add(suite, Test_node_add_parent, NULL, NULL);
	Test_add(suite, Test_node_print_tag, NULL, (const void *) &tag_node);
	Test_add(suite, Test_node_print_content, NULL, (void *) &content_node);
	Test_add(suite, Test_node_free, (void *) &tag_node, NULL);

	Test_all_run(suite);
	Test_print_results(suite);

	Test_free(&suite);
	Tag_lookup_free(&tbl);

	return 0;
}

static int Test_node_new_tag(T t, void *s, const void *chk)
{
	Node_T *tn = (Node_T *) s;
	const char *n = (const char *) chk;

	assert(n && *n);

	*tn = Node_new_tag(Tag_lookup_tag(&tbl, n), n, NULL);
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

	assert(C_TAGND == Node_type(*tn));

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_node_type_content(T t, void *s, const void *chk)
{
	Node_T *cn = (Node_T *) chk;

	assert(C_CNTND == Node_type(*cn));

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
	Node_T em = Node_new_tag(Tag_lookup_tag(&tbl, "em"), "em", NULL);

	Node_add_sibling(cn, &em);

	TEST_FUNC_NAME(t);

	return Node_has_sibling(cn);
}

static int Test_node_parent(T t, void *s, const void *chk)
{
	int f = 0;

	Attr_list_T alist = Attr_list_list();
	Attr_rep_T attr1 = Attr_rep_new("id", "loading");
	Node_T pn;
	Node_T cn = Node_new_tag(Tag_lookup_tag(&tbl, "h1"), "h1", NULL);
	Node_T *pnp = NULL;

	alist = Attr_list_enqueue(alist, attr1);
	pn = Node_new_tag(Tag_lookup_tag(&tbl, "body"), "body", &alist);

	Node_add_child(&pn, &cn); 		// add cn as child to pn
	pnp = Node_parent(&cn);			// get parent of cn

	Fmt_fprint(stdout, "pn: '%s'\n", Node_print(&pn));

	f = !strcmp(Node_name(pn), Node_name(*pnp)); 

	TEST_FUNC_NAME(t);

	Node_free(&pn);
	Node_free(&cn);

	return f;
}

static int Test_node_back(T t, void *s, const void *chk)
{
	int f1, f2  = 0;
	Node_T pn = Node_new_tag(Tag_lookup_tag(&tbl, "body"), "body", NULL);
	Node_T cn = Node_new_tag(Tag_lookup_tag(&tbl, "h1"), "h1", NULL);
	Node_T sn = Node_new_tag(Tag_lookup_tag(&tbl, "p"), "p", NULL);
	Node_T *bnp = NULL;

	Node_add_child(&pn, &cn); 		// add cn as child to pn
	bnp = Node_back(&cn);

	f1 = !strcmp(Node_print(&pn), Node_print(bnp));

	Node_add_sibling(&cn, &sn);
	bnp = Node_back(&sn);

	f2 = !strcmp(Node_print(&cn), Node_print(bnp));

	Node_free(&pn);
	Node_free(&cn);
	Node_free(&sn);

	TEST_FUNC_NAME(t);

	return f1 && f2;
}

static int Test_node_add_parent(T t, void *s, const void *chk)
{
	int f = 0;
	Node_T pn = Node_new_tag(Tag_lookup_tag(&tbl, "head"), "head", NULL);
	Node_T cn = Node_new_tag(Tag_lookup_tag(&tbl, "title"), "title", NULL);
	Node_T sn = Node_new_tag(Tag_lookup_tag(&tbl, "link"), "link", NULL);
	Node_T *pnp = NULL;

	Node_add_child(&pn, &cn); 		// add cn as child of pn
	Node_add_sibling(&cn, &sn); 		// add sn as sibling of cn
	Node_add_parent(&pn, &sn); 		// hookup parent of sn

	pnp = Node_parent(&sn);

	f = !strcmp(Node_print(&pn), Node_print(pnp));
	
	TEST_FUNC_NAME(t);

	Node_free(&pn);
	Node_free(&cn);
	Node_free(&sn);

	return f;
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
