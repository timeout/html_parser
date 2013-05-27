#include <string.h>
#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_fmt.h"
#include "html_parser_attribute_rep.h"
#include "html_parser_tester.h"

#define T Test_T

/* test data */
struct test_att_data {
	char *name;
	char *value;
};

struct name_data { 	/* positive and negative matches */
	const char *p;
	const char *n;
};

struct val_data {
	const char *p;
	const char *n;
};

/* test functions */
static int Test_attr_rep_new(T t, void *, const void *);
static int Test_attr_rep_clone(T t, void *, const void *);
static int Test_attr_rep_name(T t, void *, const void *);
static int Test_attr_rep_value(T t, void *, const void *);
static int Test_attr_rep_change_value(T t, void *, const void *);
static int Test_attr_rep_isname(T t, void *, const void *);
static int Test_attr_rep_print(T t, void *, const void *);
static int Test_attr_rep_free(T t, void *, const void *);

int main(int argc, const char *argv[])
{
	T suite;
	Attr_rep_T a = NULL;
	struct test_att_data *td;
	struct name_data *nd;
	struct val_data *vd;

	NEW(td);
	td->name = "class";
	td->value = "block block-search";

	NEW(nd);
	nd->p = "class";
	nd->n = "klasse";

	NEW(vd);
	vd->p = "block block-search";
	vd->n = "block change-value";

	Fmt_fprint(stderr, "==> Starting %s <==\n", __FILE__);

	suite = Test_init();
	Test_add(suite, Test_attr_rep_new, (void *) &a, (const void *) td);
	Test_add(suite, Test_attr_rep_clone, PREV_INPUT(suite), NULL);
	Test_add(suite, Test_attr_rep_name, Test_input(suite, Test_curr(suite)), 
			(const void *) nd);
	Test_add(suite, Test_attr_rep_value, Test_input(suite, Test_curr(suite)), 
				(const void *) vd);
	Test_add(suite, Test_attr_rep_change_value, Test_input(suite, Test_curr(suite)), 
				(const void *) vd);
	Test_add(suite, Test_attr_rep_isname, Test_input(suite, Test_curr(suite)),
				(const void *) nd);
	Test_add(suite, Test_attr_rep_print, Test_input(suite, Test_curr(suite)), 
			NULL);
	Test_add(suite, Test_attr_rep_free,Test_input(suite, Test_curr(suite)),
			NULL);

	Test_all_run(suite);
	Test_all_results(suite);
	Test_print_results(suite);

	Test_free(&suite);
	FREE(td);
	FREE(nd);
	FREE(vd);

	return 0;
}

static int Test_attr_rep_new(T t, void *s, const void *chk)
{
	Attr_rep_T *a = (Attr_rep_T *) s;
	struct test_att_data *td = (struct test_att_data *) chk;

	*a = Attr_rep_new(td->name, td->value);
	assert(a && *a);

	TEST_FUNC_NAME(t);
	TEST_FUNC_OUT(t, Attr_rep_print(*a));

	return TEST_SUCCESS;
}

static int Test_attr_rep_clone(T t, void *s, const void *chk)
{
	Attr_rep_T cl;
	Attr_rep_T *a = (Attr_rep_T *) s;

	assert(a && *a);
	TEST_FUNC_NAME(t);

	cl = Attr_rep_clone(*a);
	assert(cl);

	Attr_rep_change_value(cl, "block block-found");
	TEST_FUNC_OUT(t, Fmt_string("%s", Attr_rep_print(cl)));

	Attr_rep_free(&cl);

	return TEST_SUCCESS;
}

static int Test_attr_rep_name(T t, void *s, const void *chk)
{
	Attr_rep_T *a = (Attr_rep_T *) s;
	struct name_data *nd = (struct name_data *) chk;

	assert(a && *a);
	TEST_FUNC_NAME(t);

	return strcmp(nd->p, Attr_rep_name(*a)) == 0
		&& !strcmp(nd->n, Attr_rep_name(*a)) == 0;
}

static int Test_attr_rep_value(T t, void *s, const void *chk)
{
	Attr_rep_T *a = (Attr_rep_T *) s;
	struct name_data *vd = (struct name_data *) chk;

	assert(a && *a);
	TEST_FUNC_NAME(t);

	return strcmp(vd->p, Attr_rep_value(*a)) == 0
		&& !strcmp(vd->n, Attr_rep_value(*a)) == 0;
}

static int Test_attr_rep_change_value(T t, void *s, const void *chk)
{
	Attr_rep_T *a = (Attr_rep_T *) s;
	struct name_data *vd = (struct name_data *) chk;

	assert(a && *a);
	TEST_FUNC_NAME(t);

	Attr_rep_change_value(*a, (char *) vd->n);

	TEST_FUNC_OUT(t, Attr_rep_print(*a));

	return TEST_SUCCESS;
}

static int Test_attr_rep_isname(T t, void *s, const void *chk)
{
	Attr_rep_T *a = (Attr_rep_T *) s;
	struct name_data *nd = (struct name_data *) chk;

	assert(a && *a);
	TEST_FUNC_NAME(t);

	return Attr_rep_isname(*a, nd->p)
		&& !Attr_rep_isname(*a, nd->n);
}

static int Test_attr_rep_print(T t, void *s, const void *chk)
{
	Attr_rep_T *a = (Attr_rep_T *) s;

	assert(a && *a);
	TEST_FUNC_NAME(t);
	TEST_FUNC_OUT(t, Attr_rep_print(*a));

	return TEST_SUCCESS;
}

static int Test_attr_rep_free(T t, void *s, const void *chk)
{
	Attr_rep_T *a = (Attr_rep_T *) s;

	TEST_FUNC_NAME(t);

	Attr_rep_free(a);
	assert(*a == NULL);

	return TEST_SUCCESS;
}
