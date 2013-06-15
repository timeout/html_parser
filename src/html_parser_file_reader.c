#include <stdio.h>
#include <string.h>
#include <err.h>
#include <errno.h>

#include "html_parser_mem.h"
#include "html_parser_text.h"
#include "html_parser_fmt.h"
#include "html_parser_file_reader.h"

/* static function prototypes */

Text_T File_reader_reader(const char *pathname, const char *filter)
{
	int c, i, max = BUFFSIZE;
	char *buf = ALLOC(max);
	Text_T doc;
	FILE *in;
 
	// debug
	Fmt_register('T', Text_fmt);
	Fmt_fprint(stderr, "%s\n", pathname);
	Fmt_fprint(stderr, "BUFFSIZE: %d\n", BUFFSIZE);

	if ((in = fopen(pathname, "r")) == NULL)
		err(1, "%s", pathname);

	for (c = fgetc(in), i = 0; c != EOF; c = fgetc(in), i++) {
		if (i == max) { 		// max buf
			RESIZE(buf, max <<= 1);
		}

		buf[i] = c;
	}

	doc = Text_put(buf);

	Fmt_fprint(stderr, "%T\n", &doc);

	return doc;
}

