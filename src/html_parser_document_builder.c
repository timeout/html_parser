
#include "html_parser_mem.h"
#include "html_parser_atom.h"
#include "html_parser_text.h"
#include "html_parser_fmt.h"
#include "html_parser_types.h"
#include "html_parser_document_node.h"
#include "html_parser_tag_reader.h"
#include "html_parser_attribute_list.h"
#include "html_parser_attribute_reader.h"
#include "html_parser_document_tree.h"
#include "html_parser_document_builder.h"

Doc_tree_T *Doc_builder_builder(Text_T *doc)
{
	Doc_tree_T *dt;
	Text_T chunk;
	Lookup_T tbl = Tag_lookup_init();

	NEW(dt);
	*dt = Doc_tree_tree();	

	while ((*doc).len > 0) {
		char *tag;
		const char *content;
		Attr_list_T *attrs = NULL;
		Node_T *n;
		Tag_E tf;
		Chunk_E cf = Doc_builder_sieve(doc, &chunk);


		switch (cf) {
			case C_TAGND:
				tf = Tag_reader_reader(&chunk, &tag, &tbl);

				if (tf & T_YATTR) {
					NEW(attrs);
					*attrs = Attr_list_list();
					*attrs = Attribute_reader(&chunk);
				}

				if (tf & T_OPTNL || tf & T_STRCT || tf & T_VOID) {
					NEW(n);
					*n = Node_new_tag(tf, Atom_str(tag), attrs);
					Fmt_fprint(stderr, "==========\n");
					Fmt_fprint(stderr, "[ 2. node: '%s' ]\n",
							Node_print(n));
					Fmt_fprint(stderr, "[ insert tf: %s, tag:" 
							"'%s' ]\n",
							Tag_type_rep(tf), Atom_str(tag));
					Fmt_fprint(stderr, "==========\n");
					Doc_tree_insert(*dt, n);
				} else if (tf & T_CLOSE) {
					Doc_tree_end_tag(*dt, tag);
				} else {
					Fmt_fprint(stdout, "\033[31mWARNING: '%s'" 
							" (%s)\033[0m\n", 
							tag, Tag_type_rep(tf));
				}

				FREE(tag);
				break;
			case C_CNTND:
				content = Text_get(NULL, -1, chunk);
				NEW(n);
				*n = Node_new_content(content);
				Doc_tree_insert(*dt, n);
				break;
			case C_CNTMT:
			default:			/* C_ERROR */
				;
		}
	}

	return dt;
}

Chunk_E Doc_builder_sieve(Text_T *doc, Text_T *chunk)
{
	Chunk_E f;
	int open = Text_chr(*doc, 1, 0, '<');
	int close = Text_chr(*doc, 1, 0, '>');

	if (open == 0) {
		if (close) { 		/* last tag */
			*chunk = Text_sub(*doc, 1, close);
			f = C_TAGND;
			*doc = Text_sub(*doc, close + 1, 0);
		} else { 		/* is there anything left? */
			*chunk = *doc;
			*doc = Text_sub(*doc, 0, 0);
			if (Text_upto(*chunk, 1, 0, Text_nonwhite)) {
				f = C_CNTND;
			} else {
				f = C_CNTMT;
			}
		}
		*doc = Text_sub(*doc, 0, 0);
	} else if (close == 0) {
		f = C_ERROR;
	} else if (open < close) { 	/* content */
		if (Text_upto(*doc, 1, open, Text_nonwhite)) {
			*chunk = Text_sub(*doc, 1, open);
			f = C_CNTND;
		} else {
			f = C_CNTMT;
		}
		*doc = Text_sub(*doc, open + 1, 0);
	} else {
		*chunk = Text_sub(*doc, 1, close);
		f = C_TAGND;
		*doc = Text_sub(*doc, close + 1, 0);
	}

	return f;
}

const char *Chunk_type_rep(Chunk_E c)
{
	if (c & C_TAGND) return "C_TAGND";
	if (c & C_CNTND) return "C_CNTND";
	if (c & C_CNTMT) return "C_CNTMT";
	return "C_ERROR";

}
