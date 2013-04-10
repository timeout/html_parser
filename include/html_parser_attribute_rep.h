#ifndef __ATTRIBUTE_REP_INCLUDED__
#define __ATTRIBUTE_REP_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#define T Attr_rep_T

typedef struct T *T;

extern T Attr_rep_new(char *name, char *value);
extern const char *Attr_rep_name(T attr);
extern const char *Attr_rep_value(T attr);
extern char *Attr_rep_change_value(T attr, char *new_value);
extern int Attr_rep_isname(T attr, const char *name);
extern const char *Attr_rep_print(T attr);
extern void Attr_rep_free(T *attr);

#undef T

#ifdef __cplusplus
}
#endif
#endif /* __ATTRIBUTE_REP_INCLUDED__ */
