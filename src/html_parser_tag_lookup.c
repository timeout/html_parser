#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "html_parser_types.h"
#include "html_parser_mem.h"
#include "html_parser_assert.h"
#include "html_parser_set.h"
#include "html_parser_text.h"
#include "html_parser_tag_lookup.h"

#define T Lookup_T

#define ARRAY_SIZE(x) ((sizeof x) / (sizeof *x))

/* static function prototypes */
static int string_cmp(const void *str1, const void *str2);
static unsigned string_hash(const void *str);

struct T{
	Set_T opt_set;
	Set_T void_set;
	Set_T strict_set;
};

static char *opt_void_names[] = {

	"html", "head", "body", "li", "option", "p", "tbody", "td",
	"tfoot", "th", "thead", "tr" , NULL

};

static char *void_names[] = {

	"area", "br", "base", "col", "frame", "hr", "img", "input",
	"link", "meta", "param" , NULL

};

static char *strict_names[] = {

	"a", "abbr", "acronym", "address", "b", "bdo", "big",
	"blockquote", "button", "caption", "center", "cite", "code",
	"del", "dfn", "div", "dl", "dt", "em", "fieldset", "form",
	"frameset", "h1", "h2", "h3", "h4", "h5", "h6", "i", "ins",
	"kbd", "label", "legend", "map", "noframes", "noscript",
	"object", "ol", "optgroup", "pre", "q", "samp", "script",
	"select", "small", "span", "strike", "strong", "style", "sub",
	"sup", "table", "textarea", "title", "tt", "ul", "var", NULL

};

T Tag_lookup_init(void)
{
	T nametbl;

	NEW(nametbl);

	nametbl->opt_set = Set_new(ARRAY_SIZE(opt_void_names), string_cmp, string_hash);
	nametbl->void_set = Set_new(ARRAY_SIZE(void_names), string_cmp, string_hash);
	nametbl->strict_set = Set_new(ARRAY_SIZE(strict_names), string_cmp, string_hash);

	for (int i = 0; opt_void_names[i] != NULL; i++)
		Set_put(nametbl->opt_set, opt_void_names[i]);

	for (int i = 0; void_names[i] != NULL; i++)
		Set_put(nametbl->void_set, void_names[i]);

	for (int i = 0; strict_names[i] != NULL; i++)
		Set_put(nametbl->strict_set, strict_names[i]);

	return nametbl;
}

void Tag_lookup_free(T *t)
{
	assert(t && *t);

	Set_free(&(*t)->opt_set);
	Set_free(&(*t)->void_set);
	Set_free(&(*t)->strict_set);

	FREE(*t);
}

Tag_E Tag_lookup_tag(T *t, const char *name)
{
	assert(t && *t);
	assert(name);

	assert((*t)->opt_set);
	assert((*t)->void_set);
	assert((*t)->strict_set);

	if (Set_member((*t)->opt_set, name)) return T_OPTNL;
	if (Set_member((*t)->void_set, name)) return T_VOID;
	if (Set_member((*t)->strict_set, name)) return T_STRCT;
		
	return T_UNKWN;
}

static int string_cmp(const void *str1, const void *str2)
{
	return strcmp((const char *) str1, (const char *) str2);
}

/* one-at-a-time hash:
 * http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 */
static unsigned string_hash(const void *str)
{
	int len = strlen((const char *) str);
	unsigned char *p = (unsigned char *) str;
	unsigned h = 0;
	int i;

	for ( i = 0; i < len; i++ ) {
		h += p[i];
		h += ( h << 10 );
		h ^= ( h >> 6 );
	}

	h += ( h << 3 );
	h ^= ( h >> 11 );
	h += ( h << 15 );

	return h;
}

const char *Tag_type_rep(Tag_E t)
{
	Text_T rep = Text_box("", 0);

	if (t & T_YATTR)
		rep = Text_cat(rep, Text_put("T_YATTR | "));
	if (t & T_NATTR)
		rep = Text_cat(rep, Text_put("T_NATTR | "));
	if (t & T_OPTNL)
		rep = Text_cat(rep, Text_put("T_OPTNL | "));
	if (t & T_STRCT)
		rep = Text_cat(rep, Text_put("T_STRCT | "));
	if (t & T_VOID)
		rep = Text_cat(rep, Text_put("T_VOID | "));
	if (t & T_CMMNT)
		rep = Text_cat(rep, Text_put("T_CMMNT | "));
	if (t & T_INSTR)
		rep = Text_cat(rep, Text_put("T_INSTR | "));
	if (t & T_CLOSE)
		rep = Text_cat(rep, Text_put("T_CLOSE | "));
	if (t & T_UNKWN)
		rep = Text_cat(rep, Text_put("T_UNKWN | "));

	return Text_get(NULL, -1, Text_sub(rep, 1, -3));
}
