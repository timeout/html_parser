#include <stdio.h>
#include <string.h>
#include <err.h>
#include <errno.h>

#include "html_parser_mem.h"
#include "html_parser_text.h"
#include "html_parser_fmt.h"
#include "html_parser_file_reader.h"

/* static function prototypes */

Text_T *File_reader_reader(const char *pathname, const char *filter)
{
	int c, i = 0;
	char buf[BUFSIZ];
	Text_T *f;
	FILE *in;
 
	NEW(f);
	*f = Text_box("", 0);

	Fmt_register('T', Text_fmt);
	Fmt_fprint(stderr, "%s\n", pathname);
	Fmt_fprint(stderr, "BUFSIZ: %d\n", BUFSIZ);

	if ((in = fopen(pathname, "r")) == NULL)
		err(1, "%s", pathname);

	while((c = getc(in)) != EOF) {
		if (filter == NULL)
			buf[i++] = c;
		else if (strchr(filter, c) == NULL) 
			buf[i++] = c;

		if (i == BUFSIZ) {
			i = 0;
			*f = Text_cat(*f, Text_box(buf, BUFSIZ));
		}
	}
	*f = Text_cat(*f, Text_box(buf, i));

	Fmt_fprint(stderr, "%T\n", f);

	return f;
}

