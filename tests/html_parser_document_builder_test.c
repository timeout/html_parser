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
	
	Fmt_fprint(stderr, "==> Starting %s <==\n", __FILE__);

	suite = Test_init();

	Test_add(suite, Test_doc_build_sieve, NULL, NULL);
	Test_add(suite, Test_doc_build_builder, NULL, NULL);

	Test_all_run(suite);
	Test_print_results(suite);

	Test_free(&suite);

	return 0;
}

static int Test_doc_build_sieve(T t, void *s, const void *chk)
{
	Chunk_E f;
	Text_T chunk;

	Text_T doc = File_reader_reader("/home/joe/programming/c/html_parser/data/doc_build_sieve_test.html", NULL);
	/*Text_T doc = File_reader_reader("data/arni.html", NULL);*/

	while (doc.len > 0) {
		f = Doc_builder_sieve(&doc, &chunk);
		if (!(f & C_CNTMT)) {
			Fmt_fprint(stderr, "\033[32m%s\033[m => '%T'\n", 
					Chunk_type_rep(f), &chunk);
		}
	}

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_doc_build_builder(T t, void *s, const void *chk)
{
	Doc_tree_T tr;
	char *str;
	/*Text_T doc = File_reader_reader("data/file_reader_test.html", NULL);*/
	Text_T doc = File_reader_reader("data/doc_build_sieve_test.html", NULL);
	/*Text_T doc = File_reader_reader("data/arni.html", NULL);*/

	tr = Doc_builder_builder(doc);

	TEST_FUNC_NAME(t);
	/*TEST_FUNC_OUT(t, Doc_tree_print(*dt));*/

	Fmt_fprint(stdout, "==========\n");

	Doc_tree_print(tr);
	// Doc_tree_free(dt);

	return TEST_FAIL;
}
