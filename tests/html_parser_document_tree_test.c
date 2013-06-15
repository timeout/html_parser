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

/* test data */
Lookup_T tbl;

/* test functions */
static int Test_doc_tree_insert(T t, void *, const void *);
static int Test_doc_tree_insert_contents(T t, void *, const void *);
static int Test_doc_tree_insert_void(T t, void *, const void *);
static int Test_doc_tree_insert_strict(T t, void *, const void *);
static int Test_doc_tree_insert_optnl1(T t, void *, const void *);
static int Test_doc_tree_insert_optnl2(T t, void *, const void *);
static int Test_doc_tree_end_root(T t, void *, const void *);
static int Test_doc_tree_end_simple(T t, void *, const void *);
static int Test_doc_tree_end_child(T t, void *, const void *);
static int Test_doc_tree_end_optopt(T t, void *, const void *);

int main(int argc, const char *argv[])
{
	T suite;

	tbl = Tag_lookup_init();

	Fmt_fprint(stderr, "==> Starting %s <==\n", __FILE__);

	suite = Test_init();

	Test_add(suite, Test_doc_tree_insert, NULL, NULL);
	Test_add(suite, Test_doc_tree_insert_contents, NULL, NULL);
	Test_add(suite, Test_doc_tree_insert_void, NULL, NULL);
	Test_add(suite, Test_doc_tree_insert_strict, NULL, NULL);
	Test_add(suite, Test_doc_tree_insert_optnl1, NULL, NULL);
	Test_add(suite, Test_doc_tree_insert_optnl2, NULL, NULL);
	Test_add(suite, Test_doc_tree_end_root, NULL, NULL);
	Test_add(suite, Test_doc_tree_end_simple, NULL, NULL);
	Test_add(suite, Test_doc_tree_end_child, NULL, NULL);
	Test_add(suite, Test_doc_tree_end_optopt, NULL, NULL);

	Test_all_run(suite);
	Test_print_results(suite);

	Test_free(&suite);
	Tag_lookup_free(&tbl);

	return 0;
}

static int Test_doc_tree_insert(T t, void *s, const void *chk)
{
	Node_T n = Node_new_tag(Tag_lookup_tag(&tbl, "html"), "html", NULL);
	Doc_tree_T tr = Doc_tree_insert(NULL, &n);

	assert(tr);

	TEST_FUNC_NAME(t);

	Node_free(&n);

	return TEST_SUCCESS;
}

static int Test_doc_tree_insert_contents(T t, void *s, const void *chk)
{
	int f = 0;
	char *p_name, *b_name, *c_name, *n_name;
	Doc_tree_T tr;

	Node_T n = Node_new_tag(Tag_lookup_tag(&tbl, "html"), "html", NULL);
	Node_T cn = Node_new_content("I'm Winston Wolfe. I solve problems.");
	Node_T *np;

	p_name = b_name = c_name = n_name = NULL;

	n_name = (char *) Node_print(&n);

	tr = Doc_tree_insert(NULL, &n); 		// init tree
	Doc_tree_insert(tr, &cn); 

	np = Node_parent(&cn);
	p_name = (char *) Node_print(np); 		// parent

	np = Node_back(&cn);
	b_name = (char *) Node_print(np); 		// back link
	
	/*Fmt_fprint(stdout, "parent: %s\n", p_name);*/
	/*Fmt_fprint(stdout, "back: %s\n", b_name);*/

	f = !strcmp(p_name, n_name); 			// parent should be n

	np = Doc_tree_curr(tr);
	c_name = (char *) Node_print(np); 		// curr in tree
	/*Fmt_fprint(stdout, "curr: %s\n", c_name);*/

	f = f & !strcmp(c_name, n_name); 		// curr should be n

	TEST_FUNC_NAME(t);

	FREE(p_name);
	FREE(b_name);
	FREE(c_name);
	FREE(n_name);

	Node_free(&n);
	Node_free(&cn);
	
	return f;
}

static int Test_doc_tree_insert_void(T t, void *s, const void *chk)
{
	int f = 0;
	char *p_name, *b_name, *c_name, *n_name, *s_name;
	Doc_tree_T tr;

	Node_T n = Node_new_tag(Tag_lookup_tag(&tbl, "head"), "head", NULL);
	Node_T v1 = Node_new_tag(Tag_lookup_tag(&tbl, "link"), "link", NULL);
	Node_T v2 = Node_new_tag(Tag_lookup_tag(&tbl, "meta"), "meta", NULL);

	Node_T *np;

	p_name = b_name = c_name = n_name = s_name = NULL;

	n_name = (char *) Node_print(&n);

	tr = Doc_tree_insert(NULL, &n); 		// init tree
	Doc_tree_insert(tr, &v1); 			// first void node
	Doc_tree_insert(tr, &v2);  			// second void node

	np = Node_parent(&v1);
	p_name = (char *) Node_print(np); 		// parent

	np = Node_back(&v1);
	b_name = (char *) Node_print(np); 		// back link
	
	/*Fmt_fprint(stdout, "parent: %s\n", p_name);*/
	/*Fmt_fprint(stdout, "back: %s\n", b_name);*/

	f = !strcmp(p_name, n_name); 			// parent should be n

	if (Node_has_sibling(&v1)) { 			// sibling should be meta
		np = Node_sibling(&v1);
		s_name = (char *) Node_print(np);
		/*Fmt_fprint(stdout, "sibling: %s\n", s_name);*/
	}

	np = Doc_tree_curr(tr);
	c_name = (char *) Node_print(np); 		// curr in tree
	/*Fmt_fprint(stdout, "curr: %s\n", c_name);*/

	f = f & !strcmp(c_name, n_name); 		// curr should be n

	TEST_FUNC_NAME(t);

	Doc_tree_print(tr);
	Fmt_fprint(stdout, "==========\n\n");

	FREE(p_name);
	FREE(b_name);
	FREE(c_name);
	FREE(n_name);
	FREE(s_name);

	Node_free(&n);
	Node_free(&v1);
	Node_free(&v2);
	
	return f;
}

static int Test_doc_tree_insert_strict(T t, void *s, const void *chk)
{
	int f = 0;

	Node_T n = Node_new_tag(Tag_lookup_tag(&tbl, "p"), "p", NULL);
	Node_T cn1 = Node_new_content("Just because you are a character"
			" doesn't mean that you have character.");
	Node_T st = Node_new_tag(Tag_lookup_tag(&tbl, "em"), "em", NULL);
	Node_T cn2 = Node_new_content("It's your future... I see a cab ride."); 

	/* create tree */
	Doc_tree_T tr = Doc_tree_insert(Doc_tree_insert(Doc_tree_insert(Doc_tree_insert(
						NULL, &n), &cn1), &st), &cn2);

	char *n_name = (char *) Node_print(&n);
	char *b_node = (char *) Node_print(Node_back(&st)); 		// sibling of cn1
	char *p_name = (char *) Node_print(Node_parent(&st)); 		
	char *c_name = (char *) Node_print(Doc_tree_curr(tr)); 		// current is em

	f = !strcmp(n_name, p_name); 		// parent is p

	TEST_FUNC_NAME(t);

	Doc_tree_print(tr);
	Fmt_fprint(stdout, "==========\n\n");

	FREE(n_name);
	FREE(b_node);
	FREE(p_name);
	FREE(c_name);

	Node_free(&n);
	Node_free(&cn1);
	Node_free(&st);
	Node_free(&cn2);

	return f;
}

/* top level: no parent */
static int Test_doc_tree_insert_optnl1(T t, void *s, const void *chk)
{
	int f = 0;
	Doc_tree_T tr;
	char *n_name, *b_name;

	Node_T opt1 = Node_new_tag(Tag_lookup_tag(&tbl, "html"), "html", NULL);
	Node_T opt2 = Node_new_tag(Tag_lookup_tag(&tbl, "head"), "head", NULL);
	Node_T opt3 = Node_new_tag(Tag_lookup_tag(&tbl, "body"), "body", NULL);

	/* create tree */
	tr = Doc_tree_insert(Doc_tree_insert(Doc_tree_insert(NULL, &opt1), &opt2), &opt3);

	n_name = (char *) Node_print(Node_sibling(&opt1));
	b_name = (char *) Node_print(Node_back(&opt3));

	f = !strcmp(n_name, b_name); 		// all nodes are siblings

	TEST_FUNC_NAME(t);

	Doc_tree_print(tr);
	Fmt_fprint(stdout, "==========\n\n");

	FREE(n_name);
	FREE(b_name);

	Node_free(&opt1);
	Node_free(&opt2);
	Node_free(&opt3);

	return f;
}

/* internal node: parent */
static int Test_doc_tree_insert_optnl2(T t, void *s, const void *chk)
{
	int f = 0;

	Node_T st = Node_new_tag(Tag_lookup_tag(&tbl, "div"), "div", NULL);
	Node_T opt1 = Node_new_tag(Tag_lookup_tag(&tbl, "p"), "p", NULL);
	Node_T opt2 = Node_new_tag(Tag_lookup_tag(&tbl, "p"), "p", NULL);

	/* create tree */
	Doc_tree_T tr = Doc_tree_insert(Doc_tree_insert(Doc_tree_insert(
					NULL, &st), &opt1), &opt2);

	char *p1_name = (char *) Node_print(Node_parent(&opt1));
	char *p2_name = (char *) Node_print(Node_parent(&opt2));
	char *s_name = (char *) Node_print(Node_sibling(&opt1));
	char *n_name = (char *) Node_print(Doc_tree_curr(tr));

	f = !strcmp(p1_name, p2_name); 		// both ps have the same parent
	f &= !strcmp(s_name, n_name); 		// ps are siblings

	TEST_FUNC_NAME(t);

	Doc_tree_print(tr);
	Fmt_fprint(stdout, "==========\n\n");

	FREE(p1_name);
	FREE(p2_name);
	FREE(s_name);
	FREE(n_name);

	Node_free(&st);
	Node_free(&opt1);
	Node_free(&opt2);

	return f;
}

static int Test_doc_tree_end_root(T t, void *s, const void *chk)
{
	char *c_name;

	int f = 0;

	Node_T h = Node_new_tag(Tag_lookup_tag(&tbl, "html"), "html", NULL);
	Doc_tree_T tr = Doc_tree_insert(NULL, &h);

	Doc_tree_end(tr, Tag_lookup_tag(&tbl, "html"), "html"); 		// root

	c_name = (char *) Node_print(Doc_tree_curr(tr));

	f = !strcmp(c_name, "<html>");

	TEST_FUNC_NAME(t);	

	FREE(c_name);

	Node_free(&h);

	return f;
}

static int Test_doc_tree_end_simple(T t, void *s, const void *chk)
{
	char *c_name, *r_name;

	int f = 0;

	Node_T r = Node_new_tag(Tag_lookup_tag(&tbl, "html"), "html", NULL);
	Node_T h = Node_new_tag(Tag_lookup_tag(&tbl, "head"), "head", NULL);

	Doc_tree_T tr = Doc_tree_insert(Doc_tree_insert(NULL, &r), &h);

	Doc_tree_end(tr, Tag_lookup_tag(&tbl, "head"), "head"); 		// -> root

	c_name = (char *) Node_print(Doc_tree_curr(tr));
	r_name = (char *) Node_print(&r);

	f = !strcmp(r_name, c_name);

	TEST_FUNC_NAME(t);

	FREE(r_name);
	FREE(c_name);

	Node_free(&r);
	Node_free(&h);

	return f;
}

static int Test_doc_tree_end_child(T t, void *s, const void *chk)
{
	char *p_name, *c_name;

	int f = 0;

	Node_T p = Node_new_tag(Tag_lookup_tag(&tbl, "body"), "body", NULL);
	Node_T c = Node_new_tag(Tag_lookup_tag(&tbl, "h1"), "h1", NULL);

	Doc_tree_T tr = Doc_tree_insert(Doc_tree_insert(NULL, &p), &c);

	Doc_tree_end(tr, Tag_lookup_tag(&tbl, "h1"), "h1");

	p_name = (char *) Node_print(&p);
	c_name = (char *) Node_print(Doc_tree_curr(tr));

	f = !strcmp(p_name, c_name);

	TEST_FUNC_NAME(t);

	FREE(p_name);
	FREE(c_name);

	Node_free(&p);
	Node_free(&c);

	return f;
}

static int Test_doc_tree_end_optopt(T t, void *s, const void *chk)
{
	char *c_name, *b_name, *p_name;

	int f = 0;

	Node_T pa = Node_new_tag(Tag_lookup_tag(&tbl, "div"), "div", NULL);
	Node_T op1 = Node_new_tag(Tag_lookup_tag(&tbl, "p"), "p", NULL);
	Node_T op2 = Node_new_tag(Tag_lookup_tag(&tbl, "li"), "li", NULL);

	Doc_tree_T tr = Doc_tree_insert(Doc_tree_insert(Doc_tree_insert(
					NULL, &pa), &op1), &op2);

	Doc_tree_end(tr, Tag_lookup_tag(&tbl, "li"), "li");

	b_name = (char *) Node_print(Node_back(&op2));
	p_name = (char *) Node_print(Node_parent(&op2));
	c_name = (char *) Node_print(Doc_tree_curr(tr));

	f = !strcmp(c_name, b_name);
	f &= (strcmp(b_name, p_name) != 0);

	TEST_FUNC_NAME(t);


	FREE(c_name);
	FREE(b_name);
	FREE(p_name);

	Node_free(&pa);
	Node_free(&op1);
	Node_free(&op2);

	return f;
}
