#include <stdlib.h>
#include <stdio.h>
#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_document_node.h"
#include "html_parser_document_stack.h"

#define T Stack_T 

struct T {
	int count;
	struct elem {
		Node_T *n;
		struct elem *link;
	} *head;
};

T Stack_stack(void)
{
	T stk;

	NEW(stk);
	stk->count = 0;
	stk->head = NULL;

	return stk;
}

int Stack_empty(T stk)
{
	assert(stk);

	return (stk->count == 0);
}

void Stack_push(T stk, Node_T *n)
{
	struct elem *t;

	assert(stk);
	NEW(t);
	t->n = n;
	t->link = stk->head;
	stk->head = t;
	stk->count++;
}

Node_T *Stack_peek(T stk)
{
	assert(stk);
	assert(stk->count > 0);

	return stk->head->n;
}

int Stack_find(T stk, const char *tag_name)
{
	int index = 0; 		/* 0 : not found */

	assert(stk);

	if (!Stack_empty(stk)) {
		struct elem *t = stk->head;

		while (index++ < stk->count) {
			if (tag_name == Node_name(t->n))
				break;

			t = t->link;
		}
	}

	index = (index > stk->count) ? 0 : index;

	return index;
}

Node_T *Stack_pop(T stk)
{
	Node_T *n;
	struct elem *t;

	assert(stk);
	assert(stk->count > 0);

	t = stk->head;
	stk->head = t->link;
	stk->count--;
	n = t->n;
	FREE(t);

	return n;
}

void Stack_free(T *stk)
{
	struct elem *t, *u;

	assert(stk && *stk);

	for (t = (*stk)->head; t; t = u) {
		u = t->link;
		FREE(t);
	}

	FREE(*stk);
}

