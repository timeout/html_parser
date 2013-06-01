#include <string.h>
#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_fmt.h"
#include "html_parser_file_reader.h"
#include "html_parser_document_tree.h"
#include "html_parser_document_builder.h"
#include "html_parser_document_search.h"
#include "html_parser_tester.h"

#define T Test_T

/* test data */
typedef struct s_terms {
	const char *pos;
	const char *neg;
} s_terms_T;

/* test functions */
static int Test_doc_results(T t, void *, const void *);
static int Test_doc_search_tag(T t, void *, const void *);
static int Test_doc_search_content(T t, void *, const void *);

int main(int argc, const char *argv[])
{
	T suite;
	/*Match_T *results;*/
	Doc_tree_T *dt;
	Text_T *doc = File_reader_reader("data/doc_build_sieve_test.html", NULL);
	s_terms_T *ts, *cs;

	/* init tag search queries */
	NEW(ts);
	ts->pos = "div";
	ts->neg = "h2";

	/* init content search queries */
	NEW(cs);
	cs->pos = "music";
	cs->neg = "is the best";
	
	dt = Doc_builder_builder(doc);
	
	Fmt_fprint(stderr, "==> Starting %s <==\n", __FILE__);

	suite = Test_init();

	/*Test_add(suite, Test_doc_results, (void *) dt, (const void *) results);*/
	Test_add(suite, Test_doc_search_tag, (void *) dt, 
			(const void *) ts);
	Test_add(suite, Test_doc_search_content, PREV_INPUT(suite), 
			(const void *) cs);

	Test_all_run(suite);
	Test_print_results(suite);

	Test_free(&suite);

	return 0;
}

static int Test_doc_results(T t, void *s, const void *chk)
{

	TEST_FUNC_NAME(t);

	return TEST_FAIL;
}

static int Test_doc_search_tag(T t, void *s, const void *chk)
{
	Doc_tree_T *dt = (Doc_tree_T *) s;
	s_terms_T *ts = (s_terms_T *) chk;
	Match_T r1 = Doc_search_tag(dt, ts->pos, NULL, NULL);
	Match_T r2 = Doc_search_tag(dt, ts->neg, NULL, NULL);

	Fmt_fprint(stdout, "results: %d\n", Doc_search_result_size(r1));
	Fmt_fprint(stdout, "results: %d\n", Doc_search_result_size(r2));

	TEST_FUNC_NAME(t);

	Doc_search_free(&r1);
	Doc_search_free(&r2);

	return TEST_FAIL;
}

static int Test_doc_search_content(T t, void *s, const void *chk)
{

	TEST_FUNC_NAME(t);

	return TEST_FAIL;
}
