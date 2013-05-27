#ifndef __DOCUMENT_BUILDER_INCLUDED__
#define __DOCUMENT_BUILDER_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#include "html_parser_types.h"
#include "html_parser_document_node.h"
#include "html_parser_document_tree.h"
#include "html_parser_text.h"

extern Doc_tree_T *Doc_builder_builder(Text_T *doc);
extern Chunk_E Doc_builder_sieve(Text_T *doc, Text_T *chunk);

#ifdef __cplusplus
}
#endif

#endif /*__DOCUMENT_BUILDER_INCLUDED__*/
