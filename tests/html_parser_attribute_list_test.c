#include <string.h>
#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_fmt.h"
#include "html_parser_attribute_rep.h"
#include "html_parser_attribute_list.h"
#include "html_parser_tester.h"

#define T Test_T

#define REP_ARRAY_SIZE 5

/* test data */

/* test functions */
static int Test_attr_list_list(T t, void *, const void *);
static int Test_attr_list_enqueue(T t, void *, const void *);
static int Test_attr_list_dequeue(T t, void *, const void *);
static int Test_attr_list_length(T t, void *, const void *);
static int Test_attr_list_print(T t, void *, const void *);
static int Test_attr_list_free(T t, void *, const void *);

/* static function prototypes */
static void init_reps(Attr_rep_T **reps);

int main(int argc, const char *argv[])
{
	T suite;
	Attr_list_T h;
	Attr_rep_T *reps; 

	init_reps(&reps); 

	Fmt_fprint(stderr, "==> Starting %s <==\n", __FILE__);
	
	suite = Test_init();
	Test_add(suite, Test_attr_list_list, (void *) &h, NULL);
	Test_add(suite, Test_attr_list_enqueue, PREV_INPUT(suite),
			(const void *) reps);
	Test_add(suite, Test_attr_list_dequeue, PREV_INPUT(suite), NULL);
	Test_add(suite, Test_attr_list_length, PREV_INPUT(suite),
			(const void *) &reps);
	Test_add(suite, Test_attr_list_print, PREV_INPUT(suite), NULL);
	Test_add(suite, Test_attr_list_free, PREV_INPUT(suite), NULL);

	Test_all_run(suite);
	Test_print_results(suite);

	return 0;
}

static int Test_attr_list_list(T t, void *in, const void *chk)
{
	Attr_list_T *head = (Attr_list_T *) in;
	*head = Attr_list_list(); 
	assert(head && *head);

	TEST_FUNC_NAME(t)

	return TEST_SUCCESS;
}

static int Test_attr_list_length(T t, void *in, const void *chk)
{
	Attr_list_T *head = (Attr_list_T *) in;

	TEST_FUNC_NAME(t);

	TEST_FUNC_OUT(t, Fmt_string("Length: %d",
				Attr_list_length(*head)));

	return TEST_SUCCESS;
}

static int Test_attr_list_enqueue(T t, void *in, const void *chk)
{
	Attr_list_T *head = (Attr_list_T *) in;
	Attr_rep_T *reps = (Attr_rep_T *) chk;

	assert(head && *head);
	assert(reps && *reps);

	for (int i = 0; i < REP_ARRAY_SIZE; i++) 
		*head = Attr_list_enqueue(*head, reps[i]);

	TEST_FUNC_NAME(t)

	return TEST_SUCCESS;
}

static int Test_attr_list_dequeue(T t, void *in, const void *chk)
{
	Attr_list_T null_list, *head;
	Attr_rep_T tmp;

	head = (Attr_list_T *) in;
	assert(head && *head);

	tmp = Attr_list_dequeue(*head);
	assert(tmp);
	Attr_rep_free(&tmp);

	assert(REP_ARRAY_SIZE - 1 == Attr_list_length(*head));

	/* dequeue empty list */
	null_list = Attr_list_list();
	tmp = Attr_list_dequeue(null_list); 
	assert(tmp == NULL);
	assert(Attr_list_length(null_list) == 0);

	TEST_FUNC_NAME(t)

	return TEST_SUCCESS;
}

static int Test_attr_list_free(T t, void *in, const void *chk)
{
	Attr_list_T *head = (Attr_list_T *) in;

	Attr_list_free(head);

	assert(*head == NULL);

	TEST_FUNC_NAME(t)

	return TEST_SUCCESS;
}

static int Test_attr_list_print(T t, void *in, const void *chk)
{
	Attr_list_T *head = (Attr_list_T *) in;

	TEST_FUNC_NAME(t);
	TEST_FUNC_OUT(t, Fmt_string("%s", Attr_list_print(*head)));

	return TEST_SUCCESS;
}

static void init_reps(Attr_rep_T **reps)
{
	Attr_rep_T *tmp;

	tmp = ALLOC(REP_ARRAY_SIZE * sizeof(*tmp));

	tmp[0] = Attr_rep_new("id", "genre");
	tmp[1] = Attr_rep_new("class", "rock music");
	tmp[2] = Attr_rep_new("style", "punk");
	tmp[3] = Attr_rep_new("lang", "au");
	tmp[4] = Attr_rep_new("title", "Eternally Yours");

	*reps = tmp;
}
