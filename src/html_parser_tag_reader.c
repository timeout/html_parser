#include <err.h>

#include "html_parser_mem.h"
#include "html_parser_assert.h"
#include "html_parser_atom.h"
#include "html_parser_fmt.h"
#include "html_parser_text.h"
#include "html_parser_tag_lookup.h"
#include "html_parser_types.h"
#include "html_parser_tag_reader.h"

#define FIRST_CHAR (*tag).str[0]
#define LAST_CHAR (*tag).str[(*tag).len - 1]


extern Tag_E Tag_reader_reader(Text_T *tag, char **ch, Lookup_T *tbl)
{
	int start, end;
	Tag_E ret = 0;

	if (LAST_CHAR == '/') {
		ret = T_VOID;
		*tag = Text_sub(*tag, 1, -1);
	}

	switch(FIRST_CHAR) {
		case '!':
			ret = T_CMMNT;
			*ch = Text_get(NULL, -1, Text_sub(*tag, 4, -2));
			break;
		case '?':
			ret = T_INSTR;
			if (LAST_CHAR != '?') {
				*ch = Text_get(NULL, -1, Text_sub(*tag, 2, 0));
				warnx("WARNING: badly formed instruction?\n"
						"%s", *ch);
			} else
				*ch = Text_get(NULL, -1, Text_sub(*tag, 2, -1));
			break;
		case '/':
			ret = T_CLOSE;
			*ch = Text_get(NULL, -1, Text_sub(*tag, 2, 0));
			break;
		default:
			start = Text_upto(*tag, 1, 0, Text_nonwhite);
			end = Text_upto(*tag, start, 0, Text_white);

			*ch = Text_get(NULL, -1, Text_sub(*tag, start, end));

			ret = Tag_lookup_tag(tbl, *ch);

			if (end && (Text_upto(*tag, end, 0, Text_nonwhite) > 0))
				ret |= T_YATTR;
			else
				ret |= T_NATTR;
	}

	return ret;
}
