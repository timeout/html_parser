#include "html_parser_mem.h"
#include "html_parser_assert.h"
#include "html_parser_atom.h"
#include "html_parser_fmt.h"
#include "html_parser_text.h"
#include "html_parser_tag_reader.h"

extern tag_reader_E tag_reader(Text_T *tag, char **ch)
{
	tag_reader_E ret;

	/* comment */
	if (Text_chr(*tag, 1, 2, '!')) {
		ret = COMMENT;
		*ch = Text_get(NULL, -1, *tag);
	} else if (Text_chr(*tag, 1, 2, '?')) {
		ret = DIRECTIVE;
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
			*tag = Text_sub(*tag, end, 0); 	/* progress tag */
			ret = TAG_READ;
		} else
			ret = NO_ATTRIBUTES;
	}

	return ret;
}
