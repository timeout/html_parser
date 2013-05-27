#ifndef __TAG_TYPES_INCLUDED__
#define __TAG_TYPES_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum tag_reader_e {
	T_YATTR = 1,
	T_NATTR = 1 << 1,
	T_OPTNL = 1 << 2,
	T_STRCT = 1 << 3,
	T_VOID  = 1 << 4,
	T_CMMNT = 1 << 5,
	T_INSTR = 1 << 6,
	T_CLOSE = 1 << 7,
	T_UNKWN = 1 << 8
} Tag_E;

typedef enum {
	C_TAGND = 1,
	C_CNTND = 1 << 1,
	C_CNTMT = 1 << 2,
	C_ERROR = 1 << 3
} Chunk_E;

extern const char *Tag_type_rep(Tag_E t);
extern const char *Chunk_type_rep(Chunk_E c);

#ifdef __cplusplus
}
#endif
#endif /* __TAG_TYPES_INCLUDED__ */
