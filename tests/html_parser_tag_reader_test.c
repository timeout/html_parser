#include <string.h>
#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_fmt.h"
#include "html_parser_tag_reader.h"
#include "html_parser_tester.h"

#define T Test_T
#define TEST_TAG_SIZE 6

/* test data */

/* test functions */
static int Test_tag_reader_comment(T t, void *, const void *);
static int Test_tag_reader_directive(T t, void *, const void *);
static int Test_tag_reader_void(T t, void *, const void *);
static int Test_tag_reader(T t, void *, const void *);

int main(int argc, const char *argv[])
{
	T suite;
	char *tags[TEST_TAG_SIZE] = {
		"!--this is a comment...--", 
		"?xml version=\"1.0\" encoding=\"UTF-8\" ?",
		"p", 
		"html id=\"genre\"", 
		" h1  title=\"Eternally Yours\"",
		"br size=\"regular\"/"
	};

	Fmt_fprint(stderr, "==> Starting %s <==\n", __FILE__);

	suite = Test_init();
	Test_add(suite, Test_tag_reader_comment, NULL, (const void *) tags);
	Test_add(suite, Test_tag_reader_directive, NULL, (const void *) &tags);
	Test_add(suite, Test_tag_reader_void, NULL, (const void *) &tags);
	Test_add(suite, Test_tag_reader, NULL, (const void *) &tags);

	Test_all_run(suite);
	Test_print_results(suite);

	Test_free(&suite);

	return 0;
}

static int Test_tag_reader_comment(T t, void *s, const void *chk)
{
	char **tags = (char **) chk;

	for (int i = 0; i < TEST_TAG_SIZE; i++) {
		Text_T tmp = Text_put(tags[i]);
		char *tag = NULL;
		tag_E f = tag_reader(&tmp, &tag);

		if  (f & T_CMMNT)
			TEST_FUNC_OUT(t, Fmt_string("<!--%s-->", tag));

		FREE(tag);
	}

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_tag_reader_directive(T t, void *s, const void *chk)
{
	char **tags = (char **) chk;

	for (int i = 0; i < TEST_TAG_SIZE; i++) {
		Text_T tmp = Text_put(tags[i]);
		char *tag = NULL;
		tag_E f = tag_reader(&tmp, &tag);

		if  (f & T_INSTR)
			TEST_FUNC_OUT(t, Fmt_string("<?%s?>", tag));

		FREE(tag);
	}

	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_tag_reader_void(T t, void *s, const void *chk)
{
	char **tags = (char **) chk;

	for (int i = 0; i < TEST_TAG_SIZE; i++) {
		Text_T tmp = Text_put(tags[i]);
		char *tag = NULL;
		tag_E f = tag_reader(&tmp, &tag);

		if  (f & (T_VOID | T_YATTR))
			TEST_FUNC_OUT(t, Fmt_string("<%s [...] />", tag));

		FREE(tag);
	}


	TEST_FUNC_NAME(t);

	return TEST_SUCCESS;
}

static int Test_tag_reader(T t, void *s, const void *chk)
{
	char **tags = (char **) chk;
	Text_T fout = Text_box("<", 1);

	Fmt_register('T', Text_fmt);

	for (int i = 0; i < TEST_TAG_SIZE; i++) {
		char *chunk = NULL;
		Text_T tmp = Text_put(tags[i]);
		tag_E f = tag_reader(&tmp, &chunk);

		if (f != T_CMMNT && f != T_INSTR) {
			fout = Text_cat(fout, Text_put(chunk));
			fout = Text_cat(fout, Text_put(">\n<")); 
		}
	}

	fout = Text_sub(fout, 1, -2); 
	TEST_FUNC_NAME(t);
	TEST_FUNC_OUT(t, Fmt_string("%T", &fout));

	return TEST_SUCCESS;
}

