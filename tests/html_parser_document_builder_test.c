#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_fmt.h"
#include "html_parser_text.h"
#include "html_parser_file_reader.h"
#include "html_parser_types.h"
#include "html_parser_document_node.h"
#include "html_parser_document_tree.h"
#include "html_parser_document_builder.h"
#include "html_parser_tester.h"

#define T Test_T

/* test data */

/* test functions */
static int Test_doc_build_sieve(T t, void *, const void *);
static int Test_doc_build_builder(T t, void *, const void *);

int main(int argc, const char *argv[])
{
	T suite;
	Text_T chunk;
	/*Text_T *doc = File_reader_reader("data/doc_build_sieve_test.html", NULL);*/
	Text_T *doc = File_reader_reader("data/file_reader_test.html", NULL);
	
	Fmt_register('T', Text_fmt);
	Fmt_fprint(stderr, "==> Starting %s <==\n", __FILE__);

	suite = Test_init();

	Test_add(suite, Test_doc_build_sieve, (void *) &chunk,
			(const void *) doc);
	Test_add(suite, Test_doc_build_builder, NULL, NULL);

	Test_all_run(suite);
	Test_print_results(suite);

	Test_free(&suite);

	return 0;
}

/*extern Elem_E Doc_builder_sieve(Text_T *doc, Text_T *chunk);*/
static int Test_doc_build_sieve(T t, void *s, const void *chk)
{
	Chunk_E f = 0;
	Text_T chunk;
	Text_T *doc = (Text_T *) chk;

	*doc = Text_sub(*doc, 2, 0);

	while ((*doc).len > 0) {
		f = Doc_builder_sieve(doc, &chunk);
		Fmt_fprint(stderr, "f: %s, '%T'\n", Chunk_type_rep(f), &chunk);
	}

	TEST_FUNC_NAME(t);

	return TEST_FAIL;
}

static int Test_doc_build_builder(T t, void *s, const void *chk)
{
	Doc_tree_T *dt;
	char *str;
	/*Text_T *doc = File_reader_reader("data/file_reader_test.html", NULL);*/
	Text_T *doc = File_reader_reader("data/doc_build_sieve_test.html", NULL);
	/*Text_T *doc = File_reader_reader("data/arni.html", NULL);*/

	*doc = Text_sub(*doc, 2, 0);

	dt = Doc_builder_builder(doc);
	
	Fmt_fprint(stderr, "%s", str = Doc_tree_print(*dt));
	FREE(str);
	Fmt_fprint(stdout, "============\n");
	Fmt_fprint(stdout, "%s", str = Doc_tree_print_context(*dt));
	FREE(str);
	Fmt_fprint(stdout, "============\n");

	TEST_FUNC_NAME(t);
	/*TEST_FUNC_OUT(t, Doc_tree_print(*dt));*/

	Doc_tree_free(dt);

	return TEST_FAIL;
}
