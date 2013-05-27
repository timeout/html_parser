#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_atom.h"
#include "html_parser_fmt.h"
#include "html_parser_attribute_rep.h"

#define T Attr_rep_T

struct T {
	const char *name;
	char *value;
};

T Attr_rep_new(char *name, char *value)
{
	T new_attr;

	assert(name && *name);
	assert(value && *value);

	NEW(new_attr);

	new_attr->name = Atom_str(name);
	new_attr->value = Fmt_string("%s", value);

	return new_attr;
}

T Attr_rep_clone(T attr)
{
	T cl_attr;

	assert(attr);

	NEW(cl_attr);
	cl_attr->name = attr->name; 		/* atom */
	cl_attr->value = Fmt_string("%s", attr->value);

	return cl_attr;
}

const char *Attr_rep_name(T attr)
{
	assert(attr && attr->name);

	return attr->name;
}

const char *Attr_rep_value(T attr)
{
	assert(attr && attr->value);

	return attr->value;
}

char *Attr_rep_change_value(T attr, char *new_value)
{
	char *old_value;

	assert(attr && attr->name && attr->value);
	assert(new_value && *new_value);

	old_value = attr->value;
	attr->value = Fmt_string("%s", new_value);

	return old_value;
}

int Attr_rep_isname(T attr, const char *query)
{
	assert(attr && attr->name);
	assert(query && *query);

	return (attr->name == Atom_str(query));
}

const char *Attr_rep_print(T attr)
{
	assert(attr && attr->name && attr->value);

	return (Fmt_string("%s=\"%s\"", attr->name, attr->value));
}

void Attr_rep_free(T *attr)
{
	assert(*attr && (*attr)->name && (*attr)->value);

	FREE((*attr)->value); 
	FREE(*attr);
}

