#ifndef __TESTER_INCLUDED__
#define __TESTER_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#define TESTER_MAX_SIZE 20 	/* number of tests in suite */
#define TEST_SUCCESS 1
#define TEST_FAIL 0

#define T Test_T
typedef struct T *T;

extern T Test_init(void);
extern int Test_add(T t, int (*test_f)(T, void *, const void *),
		void *in, const void *chk);
extern int Test_size(T t);
extern int Test_curr(T t); 		/* pointer to current test */
extern void *Test_input(T t, int i);

extern int Test_run(T t, int i);
extern int *Test_all_run(T t);
extern int Test_result(T t, int i);
extern int *Test_all_results(T t);

extern void Test_set_func_name(T t, int i, char *n);
extern char *Test_get_func_name(T t, int i);
extern void Test_set_func_out(T t, int i, char *n);
extern char *Test_get_func_out(T t, int i);

extern void Test_print_result(T t, int i);
extern void Test_print_results(T t);
extern void Test_free(T *t);


#define TEST_FUNC_NAME(t) \
	Test_set_func_name((t), Test_curr((t)), Fmt_string("%s", __func__));
#define TEST_FUNC_OUT(t, msg) \
	Test_set_func_out((t), Test_curr((t)), (char *) (msg))
#define PREV_INPUT(t) \
	Test_input((t), Test_curr((t)))

#undef T

#ifdef __cplusplus
}
#endif

#endif /* __TESTER_INCLUDED__ */
