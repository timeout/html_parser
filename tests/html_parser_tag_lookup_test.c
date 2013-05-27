#include <string.h>
#include "html_parser_assert.h"
#include "html_parser_fmt.h"
#include "html_parser_types.h"
#include "html_parser_tag_lookup.h"
#include "html_parser_tester.h"

#define T Test_T

/* test data */

/* test functions */

int main(int argc, const char *argv[])
{
	T suite;
	Tag_E f, g;
	char *tb = "table";
	
	Fmt_fprint(stderr, "==> Starting %s <==\n", __FILE__);

	Lookup_T tbl = Tag_lookup_init();

	f = Tag_lookup_tag(&tbl, tb);
	f |= T_YATTR;
	Fmt_fprint(stderr, "%s -> %s\n", tb, Tag_type_rep(f));
	f = Tag_lookup_tag(&tbl, "html");
	f |= T_NATTR;
	Fmt_fprint(stderr, "%s -> %s\n", "html", Tag_type_rep(f));
	Tag_lookup_free(&tbl);

	g = f | T_YATTR;
	Fmt_fprint(stderr, "or'ing T_YATTR -> %s\n", Tag_type_rep(g));

	/*suite = Test_init();*/

	/*Test_add(suite, <func_name>, (void *) doc,*/
			/*(const void *) pathname);*/

	/*Test_all_run(suite);*/
	/*Test_print_results(suite);*/

	/*Test_free(&suite);*/

	return 0;
}
