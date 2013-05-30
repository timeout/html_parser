#include <string.h>
#include "html_parser_mem.h"
#include "html_parser_assert.h"
#include "html_parser_fmt.h"
#include "html_parser_text.h"
#include "html_parser_attribute_rep.h"
#include "html_parser_attribute_list.h"

#define T Attr_list_T

struct T {
	T next;
	Attr_rep_T attr;
};

T Attr_list_list(void)
{
	T e, p;
	NEW(e);
	NEW(p);
	e->next = p;
	p->next = p;

	return e;
}

/* static functio prototypes */
static void map(T head, void apply(Attr_rep_T *r, void *cl), void *cl);

T Attr_list_enqueue(T head, Attr_rep_T new_attr)
{
	T n, tmp;

	assert(head && head->next);

	NEW(n);
	n->attr = new_attr;

	tmp = head->next;
	head->next = n;
	n->next = tmp->next;
	tmp->next = n;

	return head;
}

Attr_rep_T Attr_list_dequeue(T head)
{
	Attr_rep_T attr;
	T tmp;

	assert(head && head->next);

	tmp = head->next->next->next;
	head->next->next->next = tmp->next;

	if (tmp == head->next) {
		head->next = tmp->next;
		return NULL;
	}

	attr = tmp->attr;
	FREE(tmp);

	return attr;
}

T Attr_list_clone(T head)
{
	T tmp;
	T cl = Attr_list_list();

	assert(head && head->next);

	for (tmp = head->next->next->next; tmp != head->next->next; tmp = tmp->next) {
		Attr_rep_T rep_cl = Attr_rep_clone(tmp->attr);
		cl = Attr_list_enqueue(cl, rep_cl);
	}
	
	assert(cl);	

	return cl;
}

int Attr_list_length(T head)
{
	int count = 0;
	T tmp;

	assert(head && head->next);

	for (tmp = head->next->next; tmp != head->next; tmp = tmp->next)
		count++;

	return count;
}

void Attr_list_free(T *head)
{
	T tmp, rest;
	
	assert(head && *head && (*head)->next);

	for (tmp = (*head)->next->next->next; tmp != (*head)->next->next; tmp = rest) {
		rest = tmp->next;
		Attr_rep_free(&tmp->attr);
	}

	FREE(tmp);
	FREE(*head);
}

const char *Attr_list_print(T head)
{
	char *str;
	T tmp;
	Text_T buf = Text_box("", 0);

	assert(head);

	if (head->next == head->next->next)
		return NULL;

	for (tmp = head->next->next->next; tmp != head->next;
			tmp = tmp->next) {
		str = (char *) Attr_rep_print(tmp->attr);
		Text_T tmp_buf = Text_box(str, strlen(str));
		tmp_buf = Text_cat(tmp_buf, Text_put(" "));
		buf = Text_cat(buf, tmp_buf);
	}

	str = (char *) Attr_rep_print(tmp->attr);
	buf = Text_cat(buf, Text_box(str, strlen(str)));

	return Text_get(NULL, -1, buf);
}

int Attr_list_search(T *attrs, const char *name, char *val)
{
	int ret = 0;
	T tmp;

	if ((*attrs)->next == (*attrs)->next->next)
		return ret;

	if (!name & !val) return ret; 	/* stupid */

	for (tmp = (*attrs)->next->next->next; !ret && tmp != (*attrs)->next;
			tmp = tmp->next) {

		Attr_rep_T a = tmp->attr;
		const char *v = Attr_rep_value(a);

		if (!name && val)
			ret = !strcmp(v, val);
		else if (name && !val)
			ret = Attr_rep_isname(a, name);
		else {
			ret = !strcmp(v, val) && Attr_rep_isname(a, name);
		}
	}

	return ret;
}

static void map(T head, void apply(Attr_rep_T *r, void *cl), void *cl)
{
	T tmp;

	if (head->next == head->next->next)
		return;

	tmp = head->next->next->next;
	while (tmp != head->next) {
		Attr_rep_T rep = tmp->attr;
		apply(&rep, cl);
		tmp = tmp->next;
	}
}

