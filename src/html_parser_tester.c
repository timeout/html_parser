#include <stdlib.h>
#include <string.h>
#include "html_parser_mem.h"
#include "html_parser_assert.h"
#include "html_parser_fmt.h"
#include "html_parser_text.h"
#include "html_parser_tester.h"

#define T Test_T

/* data */
struct T {
	int i;
	int curr;
	int results[TESTER_MAX_SIZE];
	int (*test_f[TESTER_MAX_SIZE])(T, void *, const void *);
	void *in[TESTER_MAX_SIZE];
	const void *chk[TESTER_MAX_SIZE];
	char *func_name[TESTER_MAX_SIZE];
	char *func_out[TESTER_MAX_SIZE];
};

/* static prototype functions */
static int func_name_length(T t, int i);
static int longest_func_name(T t);
static void format_func_out(Text_T m);
static int tests_passed(T t);

T Test_init(void)
{
	T t;

	NEW(t);
	t->i = 0;
	t->curr = 0;
	for (int i = 0; i < TESTER_MAX_SIZE; i++) {
		t->results[i] = 0;
		t->func_name[i] = NULL;
		t->func_out[i] = NULL;
		t->in[i] = NULL;
		t->chk[i] = NULL;
	}

	return t;
}

int Test_add(T t, int (*test_f)(T, void *, const void *),
		void *in, const void *chk)
{
	t->curr = t->i++;

	t->test_f[t->curr] = test_f;
	t->in[t->curr] = in;
	t->chk[t->curr] = chk;

	return t->curr;
}

int Test_size(T t)
{
	return t->i;
}

int Test_curr(T t)
{
	return t->curr;
}

void *Test_input(T t, int i)
{
	assert(i < t->i);
	return t->in[i];
}

int Test_run(T t, int i)
{
	assert(i < t->i);

	t->curr = i;

	return (int) t->test_f[i](t, t->in[i], t->chk[i]);
}

int *Test_all_run(T t)
{
	for (t->curr = 0; t->curr < t->i; t->curr++) 
		t->results[t->curr] = Test_run(t, t->curr);

	return t->results;
}

int Test_result(T t, int i)
{
	assert(i < t->i);

	return t->results[i]; 	/* prereq: test has been run. */
}

void Test_set_func_name(T t, int i, char *n)
{
	assert(t && n);

	t->func_name[i] = n;
}

char *Test_get_func_name(T t, int i)
{
	char *n;

	assert(i < t->i);

	if (t->func_name[i]) n = t->func_name[i];
	else n = Fmt_string("unknown");

	return n;
}

void Test_set_func_out(T t, int i, char *n)
{
	assert(t && n);

	t->func_out[i] = n;
}

char *Test_get_func_out(T t, int i)
{
	assert(i < t->i);
	
	return t->func_out[i]; 	/* don't call this with NULL */
}

int *Test_all_results(T t)
{
	return t->results;
}

void Test_print_results(T t)
{
	int size, lfn;
	Text_T pref, suff;

	Fmt_register('T', Text_fmt);

	lfn = longest_func_name(t);

	/* start building test result string: pref + suff */
	for (t->curr = 0; t->curr < t->i; t->curr++) {
		pref = Text_put(Test_get_func_name(t, t->curr));

		size = lfn - pref.len + 10;
		suff = Text_put(".");
		suff = Text_dup(suff, size);
		if (t->results[t->curr])
			suff = Text_cat(suff, Text_put("\033[32m[ OK ]\033[0m"));
		else 
			suff = Text_cat(suff, Text_put("\033[31m[ FAIL ]\033[0m"));

		pref = Text_cat(pref, suff);
		Fmt_fprint(stderr, "%T\n", &pref);

		if (t->func_out[t->curr]) {
			Text_T label = Text_put(" output => '");
			Text_T msg = Text_put(t->func_out[t->curr]);

			msg = Text_cat(Text_put("\033[34m"), msg);
			msg = Text_cat(msg, Text_put("\033[0m'"));
			msg = Text_cat(label, msg);

			if (Text_chr(msg, 1, 0, '\n'))
				format_func_out(msg);
			else 
				Fmt_fprint(stderr, "%T\n", &msg);
		}
	}

	Fmt_fprint(stderr, "--\nTests passed: [ %d / %d ]\n\n", tests_passed(t),
			Test_size(t));

}

void Test_print_result(T t, int i)
{
	int size, lfn;
	Text_T pref, suff;

	assert(i < t->i);

	Fmt_register('T', Text_fmt);

	lfn = longest_func_name(t);

	/* start building test result string: pref + suff */
	if (t->func_name[i])
		pref = Text_put(t->func_name[i]);
	else
		pref = Text_put("unknown");

	suff = Text_put(".");
	if (t->results[i]) {
		size = lfn - pref.len + 10;
		suff = Text_dup(suff, size);
		suff = Text_cat(suff, Text_put("\033[32m[ OK ]\033[0m"));
	}
	else {
		size = lfn - pref.len + 10;
		suff = Text_dup(suff, size);
		suff = Text_cat(suff, Text_put("\033[31m[ FAIL ]\033[0m"));
	}
	pref = Text_cat(pref, suff);
	Fmt_fprint(stderr, "%T\n", &pref);

	if (t->func_out[i]) {
		Text_T label = Text_put(" output => '");
		Text_T msg = Text_put(t->func_out[i]);

		msg = Text_cat(Text_put("\033[34m"), msg);
		msg = Text_cat(msg, Text_put("\033[0m'"));
		msg = Text_cat(label, msg);

		if (Text_chr(msg, 1, 0, '\n'))
			format_func_out(msg);
		else 
			Fmt_fprint(stderr, "%T\n", &msg);
	}
}

void Test_free(T *t)
{
	assert(t && *t);

	for ((*t)->curr = 0; (*t)->curr < (*t)->i; (*t)->curr++) {
		if ((*t)->func_name[(*t)->curr])
			FREE((*t)->func_name[(*t)->curr]);
		if ((*t)->func_out[(*t)->curr])
			FREE((*t)->func_out[(*t)->curr]);
	}

	FREE(*t);
}

static int func_name_length(T t, int i)
{
	assert(i < t->i);
	assert(t);

	return strlen(t->func_name[i]);
}

static int longest_func_name(T t)
{
	int l = strlen("unknown");

	for (int i = 0; i < t->i; i++) {
		l = l < func_name_length(t, i) ? func_name_length(t, i) : l;
	}

	return l;
}

static void format_func_out(Text_T m)
{
	int start, end;
	Text_T fmt = Text_box("", 0);
	Text_T inset = Text_put("\n           ");

	for (start = 1; Text_chr(m, start, 0, '\n'); start = end + 1) {
		end = Text_chr(m, start, 0, '\n');
		fmt = Text_cat(fmt, Text_sub(m, start, end));
		fmt = Text_cat(fmt, inset);
	}

	Fmt_fprint(stderr, "%T", &fmt);
	fmt = Text_sub(m, start, 0);
	Fmt_fprint(stderr, "%T\n", &fmt);
}

static int tests_passed(T t)
{
	int c;

	for (c = 0, t->curr = 0; t->curr < t->i; t->curr++)
		if (t->results[t->curr])
			c++;

	return c;
}

