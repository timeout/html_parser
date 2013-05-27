#include <stdio.h>
#include <string.h>

#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_fmt.h"
#include "html_parser_text.h"
#include "html_parser_file_reader.h"
#include "html_parser_tester.h"

#define T Test_T

/* test data */

/* test functions */
static int Test_file_reader_reader(T t, void *, const void *);

int main(int argc, const char *argv[])
{
	T suite;

	/*const char *pathname = "data/arni.html";*/
	const char *pathname = "data/file_reader_test.html";
	Text_T *doc = NULL;

	Fmt_fprint(stderr, "==> Starting %s <==\n", __FILE__);

	suite = Test_init();

	Test_add(suite, Test_file_reader_reader, (void *) doc,
			(const void *) pathname);

	Test_all_run(suite);
	Test_print_results(suite);

	Test_free(&suite);

	return 0;
}

static int Test_file_reader_reader(T t, void *s, const void *chk)
{
	Text_T *doc = (Text_T *) s;
	const char *pathname = (const char *) chk;

	Fmt_fprint(stderr, "pathname: '%s'\n", pathname);

	File_reader_reader(pathname, "\n\t");

	TEST_FUNC_NAME(t);

	return TEST_FAIL;
}
