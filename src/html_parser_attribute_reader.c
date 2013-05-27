#include "html_parser_mem.h"
#include "html_parser_text.h"
#include "html_parser_attribute_reader.h"
#include "html_parser_attribute_list.h"
#include "html_parser_attribute_rep.h"

/* static function prototypes */
static Text_T strip(Text_T *txt);
static Attr_rep_T attribute(Text_T *tag_chunk);
static char *attribute_name(Text_T *tag_chunk);
static char *attribute_value(Text_T *tag_chunk);

Attr_list_T Attribute_reader(Text_T *tag_chunk)
{
	Attr_list_T attr_list;

	Fmt_register('T', Text_fmt);

	attr_list = Attr_list_list();
	while (tag_chunk->len > 0) 
		attr_list = Attr_list_enqueue(attr_list, attribute(tag_chunk));

	return attr_list;
}

static Attr_rep_T attribute(Text_T *tag_chunk)
{
	char *name, *value;
	Attr_rep_T new;

	name = attribute_name(tag_chunk);
	value = attribute_value(tag_chunk);

	new =Attr_rep_new(name, value);

	/* not a great solution, but... */
	FREE(value);

	return new;
}

static char *attribute_name(Text_T *tag_chunk)
{
	int end;
	Text_T name;

	end = Text_chr(*tag_chunk, 1, 0, '=');
	name = Text_sub(*tag_chunk, 1, end);

	strip(&name);

	/* progress tag_chunk */
	*tag_chunk = Text_sub(*tag_chunk, end + 1, 0);

	return Text_get(NULL, -1, name);
}

static char *attribute_value(Text_T *tag_chunk)
{
	int start, end;
	Text_T value;

	start = Text_upto(*tag_chunk, 1, 0, Text_nonwhite);

	/* Is the value enclosed by quotes? */
	if (Text_chr(*tag_chunk, start, start + 1, '"')) {
		start++;
		end = Text_chr(*tag_chunk, start + 1, 0, '"');
	} else {
		end = Text_chr(*tag_chunk, start, 0, ' ');
	}

	value = Text_sub(*tag_chunk, start, end);

	/* progress tag_chunk */
	if (end == 0) 
		*tag_chunk = Text_sub(*tag_chunk, 0, 0);
	else
		*tag_chunk = Text_sub(*tag_chunk, Text_upto(*tag_chunk, end + 1, 0, 
					Text_nonwhite), 0);

	return Text_get(NULL, -1, value);
}

static Text_T strip(Text_T *txt)
{
	*txt = Text_sub(*txt, Text_upto(*txt, 1, 0, Text_nonwhite),
			Text_rupto(*txt, 1, 0, Text_nonwhite) + 1);

	return *txt;
}
