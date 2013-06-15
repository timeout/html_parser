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
static int Test_file_reader_reader_filter(T t, void *, const void *);

int main(int argc, const char *argv[])
{
	T suite;

	/*const char *pathname = "data/file_reader_test.html";*/
	const char *pathname = "data/arni.html";
	/*const char *pathname = "/home/joe/programming/web/rails/notes/learning_rails_3.txt";*/
	char *filter = "\n\t";

	Fmt_fprint(stderr, "==> Starting %s <==\n", __FILE__);

	suite = Test_init();

	Test_add(suite, Test_file_reader_reader, NULL, (const void *) pathname);
	/*Test_add(suite, Test_file_reader_reader_filter, (void *) filter,*/
			/*(const void *) pathname);*/

	Test_all_run(suite);
	Test_print_results(suite);

	Test_free(&suite);

	return 0;
}

static int Test_file_reader_reader(T t, void *s, const void *chk)
{
	const char *pathname = (const char *) chk;

	Text_T d = File_reader_reader(pathname, NULL);

	TEST_FUNC_NAME(t);
	/*TEST_FUNC_OUT(t, Text_get(NULL, -1, *d));*/

	return TEST_SUCCESS;
}

static int Test_file_reader_reader_filter(T t, void *s, const void *chk)
{
	const char *filter = (const char *) s;
	const char *pathname = (const char *) chk;

	Text_T d = File_reader_reader(pathname, filter);

	TEST_FUNC_NAME(t);
	TEST_FUNC_OUT(t, Text_get(NULL, -1, d));

	return TEST_SUCCESS;
}

