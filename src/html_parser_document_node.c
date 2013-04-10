#include <stdlib.h>
#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_fmt.h"
#include "html_parser_text.h"
#include "html_parser_attribute_list.h"
#include "html_parser_document_node.h"

#define T Node_T

/* type definition */
struct T {
	Elem_E node_t;
	struct T *sibling;
	union {
		struct tag {
			const char *name;
			Attr_list_T *attrs;
			struct T *child;
		} *tag;
		struct content {
			const char *text;
		} *content;
	} type;
};

/* static function prototypes */
static char *print_tag(T *n);
static char *print_content(T *n);

T Node_new_tag(const char *name, Attr_list_T *attrs)
{
	T node;
	struct tag *t;

	assert(name);

	NEW(node);
	node->node_t = TAG_NODE;
	node->sibling = NULL;

	NEW(t);
	t->name = name;
	t->attrs = attrs;
	t->child = NULL;

	node->type.tag = t;

	return node;
}

T Node_new_content(const char *text)
{
	T node;
	struct content *c;

	assert(text && *text);

	NEW(node);
	node->node_t = CONTENT_NODE;
	node->sibling = NULL;

	NEW(c);
	c->text = text;

	node->type.content = c;

	return node;
}

Elem_E Node_type(T n)
{
	assert(n);

	return n->node_t;
}

int Node_has_child(T *n)
{
	int ret = 0;

	assert(n && *n);

	if (((*n)->node_t == TAG_NODE) && 
			((*n)->type.tag->child != NULL))
		ret = 1;

	return ret;
}

int Node_has_sibling(T *n)
{
	assert(n && *n);

	return ((*n)->sibling != NULL);
}

T *Node_child(T *n) 		/* user error if node has no child */
{
	assert(n && *n);

	return (&(*n)->type.tag->child);
}

T *Node_sibling(T *n) 		/* user error if node has no sibling */
{
	assert(n && *n);

	return (&((*n)->sibling));
}

T *Node_last_sibling(T *n)
{
	Node_T *tmp = n;

	while (Node_has_sibling(tmp)) 
		tmp = Node_sibling(tmp);

	return tmp;
}

const char *Node_name(T *n)
{
	assert(n && *n);
	assert((*n)->node_t == TAG_NODE);

	return (*n)->type.tag->name;
}

Attr_list_T *Node_attr_list(T *n)
{
	assert(n && *n);
	assert((*n)->node_t == TAG_NODE);

	return (*n)->type.tag->attrs;
}

T *Node_add_child(T *parent, T *child)
{
	assert(parent && child);

	(*parent)->type.tag->child = *child;

	return child;
}

T *Node_add_sibling(T *first, T *second)
{
	assert(first && *first);
	assert(second && *second);

	(*first)->sibling = *second;

	return second;
}

const char *Node_print(T *n)
{
	const char *out;

	assert(*n);

	if (TAG_NODE == Node_type(*n)) {
		out = print_tag(n);
	} else {
		out = print_content(n);
	}

	return out;
}

void Node_free(T *n)
{
	assert(n && *n);

	if (TAG_NODE == Node_type(*n)) {
		/* name is an atom i.e. don't free it */
		struct tag *tmp = (*n)->type.tag;
		if (tmp->attrs)
			Attr_list_free(tmp->attrs);
		FREE(tmp);
	} else {
		struct content *tmp = (*n)->type.content;
		FREE(tmp);
	}

	FREE(*n);
}

static char *print_tag(T *n)
{
	Text_T buf = Text_box("<", 1);

	buf = Text_cat(buf, Text_put((*n)->type.tag->name));
	if ((*n)->type.tag->attrs) {
		buf = Text_cat(buf, Text_put(" "));
		buf = Text_cat(buf, Text_put(Attr_list_print(*(*n)->type.tag->attrs)));
	}
	buf = Text_cat(buf, Text_put(">"));

	return Text_get(NULL, -1, buf);
}

static char *print_content(T *n)
{
	Text_T buf = Text_put("\"");
	buf = Text_cat(buf, Text_put((*n)->type.content->text));
	buf = Text_cat(buf, Text_put("\""));

	return Text_get(NULL, -1, buf);
}
