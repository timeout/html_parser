#include "html_parser_mem.h"
#include "html_parser_text.h"
#include "html_parser_attribute_reader.h"
#include "html_parser_attribute_list.h"
#include "html_parser_attribute_rep.h"

/* static function prototypes */
static Text_T strip(Text_T *txt);
static Attr_rep_T attribute(Text_T *tag);
static char *attribute_name(Text_T *tag);
static char *attribute_value(Text_T *tag);

Attr_list_T attribute_reader(Text_T *tag)
{
	Attr_list_T attr_list;

	Fmt_register('T', Text_fmt);

	attr_list = Attr_list_list();
	while (tag->len > 0) 
		attr_list = Attr_list_enqueue(attr_list, attribute(tag));

	return attr_list;
}

static Attr_rep_T attribute(Text_T *tag)
{
	char *name, *value;
	Attr_rep_T new;

	name = attribute_name(tag);
	value = attribute_value(tag);

	new =Attr_rep_new(name, value);

	/* not a great solution, but... */
	FREE(value);

	return new;
}

static char *attribute_name(Text_T *tag)
{
	int end;
	Text_T name;

	end = Text_chr(*tag, 1, 0, '=');
	name = Text_sub(*tag, 1, end);

	strip(&name);

	/* progress tag */
	*tag = Text_sub(*tag, end + 1, 0);

	return Text_get(NULL, -1, name);
}

static char *attribute_value(Text_T *tag)
{
	int start, end;
	Text_T value;

	start = Text_upto(*tag, 1, 0, Text_nonwhite);

	/* Is the value enclosed by quotes? */
	if (Text_chr(*tag, start, start + 1, '"')) {
		start++;
		end = Text_chr(*tag, start + 1, 0, '"');
	} else {
		end = Text_chr(*tag, start, 0, ' ');
	}

	value = Text_sub(*tag, start, end);

	/* progress tag */
	if (end == 0) 
		*tag = Text_sub(*tag, 0, 0);
	else
		*tag = Text_sub(*tag, Text_upto(*tag, end + 1, 0, Text_nonwhite), 0);

	return Text_get(NULL, -1, value);
}

static Text_T strip(Text_T *txt)
{
	*txt = Text_sub(*txt, Text_upto(*txt, 1, 0, Text_nonwhite),
			Text_rupto(*txt, 1, 0, Text_nonwhite) + 1);

	return *txt;
}
