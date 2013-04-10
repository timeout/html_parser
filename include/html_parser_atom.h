#ifndef __ATOM_INCLUDED__
#define __ATOM_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

extern int Atom_length(const char *str);
extern const char *Atom_new(const char *str, int len);
extern const char *Atom_str(const char *str);
extern const char *Atom_int(long n);

#ifdef __cplusplus
}
#endif

#endif /* __ATOM_INCLUDED__ */
