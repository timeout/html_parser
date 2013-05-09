#include <err.h>

#include "html_parser_mem.h"
#include "html_parser_assert.h"
#include "html_parser_atom.h"
#include "html_parser_fmt.h"
#include "html_parser_text.h"
#include "html_parser_tag_reader.h"

#define FIRST_CHAR (*tag).str[0]
#define LAST_CHAR (*tag).str[(*tag).len - 1]


extern tag_E tag_reader(Text_T *tag, char **ch)
{
	int ret, start, end;

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

			if (end && (Text_upto(*tag, end, 0, Text_nonwhite) > 0))
					ret |= T_YATTR;
			else
				ret |= T_NATTR;
	}


	/*
	   if (Text_chr(*tag, 1, 2, '!')) {
	   ret = T_CMMNT;
	 *ch = Text_get(NULL, -1, *tag);
	 } else if (Text_chr(*tag, 1, 2, '?')) {
	 ret = T_INSTR;
	 *ch = Text_get(NULL, -1, *tag);
	 } else {
	 int start, end;
	 Text_T chunk;
	 char *tag_str;

	 start = Text_upto(*tag, 1, 0, Text_nonwhite);
	 end = Text_upto(*tag, start, 0, Text_white);

	 chunk = Text_sub(*tag, start, end);
	 tag_str = Text_get(NULL, -1, chunk);
	 *ch = (char *) Atom_str(tag_str);
	 FREE(tag_str);

	 if (end) {
	 *tag = Text_sub(*tag, end, 0); 	/<* progress tag *>/
	 ret = TAG_READ;
	 } else
	 ret = NO_ATTRIBUTES;
	 }
	 */

	return ret;
}
