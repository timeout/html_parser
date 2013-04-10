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
