#include <string.h>
#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_fmt.h"
#include "html_parser_text.h"
#include "html_parser_attribute_rep.h"
#include "html_parser_attribute_list.h"
#include "html_parser_attribute_reader.h"
#include "html_parser_tester.h"

#define T Test_T
#define ATTR_LIST_SIZE 3

/* test data */

/* test functions */
static int Test_attribute_list_reader(T t, void *, const void *);

int main(int argc, const char *argv[])
{
	Test_T suite;
	char *attrs[ATTR_LIST_SIZE] = {
		"id=\"genre\"", 
		"title=\"Eternally Yours\"",
		"rel=\"stylesheet\" type=\"text/css\" href=\"https://d11xdyzr0div58.cloudfront.net/static/archweb.78c17bb7e94a.css\" media=\"screen, projection\" title=\"Arch Linux Packages\""
	};

	Fmt_fprint(stderr, "==> Starting %s <==\n", __FILE__);

	suite = Test_init();
	Test_add(suite, Test_attribute_list_reader, NULL, (const void *) attrs);

	Test_all_run(suite);
	Test_print_results(suite);

	Test_free(&suite);

	return 0;
}

static int Test_attribute_list_reader(T t, void *s, const void *chk)
{
	char **attrs = (char **) chk;
	Text_T fout = Text_box("", 0);

	Fmt_register('T', Text_fmt);

	for(int i = 0; i < ATTR_LIST_SIZE; i++) {
		char *lr;
		Attr_list_T head = Attr_list_list();
		Text_T tmp = Text_put(attrs[i]);

		head = attribute_reader(&tmp);

		lr = (char *) Attr_list_print(head);

		fout = Text_cat(fout, Text_put(lr));
		fout = Text_cat(fout, Text_put("\n"));

		FREE(lr);
		Attr_list_free(&head);
	}

	fout = Text_sub(fout, 1, -2); 
	TEST_FUNC_NAME(t);
	TEST_FUNC_OUT(t, Fmt_string("%T", &fout));

	return TEST_SUCCESS;
}

