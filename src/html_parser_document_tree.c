#include <stdlib.h>
#include <string.h>
#include "html_parser_assert.h"
#include "html_parser_mem.h"
#include "html_parser_fmt.h"
#include "html_parser_text.h"
#include "html_parser_types.h"
#include "html_parser_document_node.h"
#include "html_parser_document_stack.h"
#include "html_parser_document_tree.h"

#define T Doc_tree_T

#define INSERT(c, node) \
	if (Node_has_child((c))) { \
		Node_add_sibling( last_sibling( Node_child((c)) ), (node) ); \
		Node_add_parent(curr, node); \
	} else { \
		Node_add_child(curr, node); \
	} \


/* type definition */
struct T {
	Node_T *root;
	Node_T *curr;
};

typedef enum { STATE_INITIAL, CHILD, SIBLING, PARENT, BACK, END, NUM_STATES } state_t;

typedef struct instance_data {
	int indent;
	Node_T *n;	
} instance_data_t;

typedef state_t state_func_t( instance_data_t *data );

/* static prototype functions */
static Node_T *last_sibling(Node_T *n);
static Node_T *insert_contents(Node_T *curr, Node_T *n);
static Node_T *insert_tag(Node_T *curr, Node_T *n);
static Node_T *insert_tag_optnl(Node_T *curr, Node_T *n);
static Node_T *insert_tag_void(Node_T *curr, Node_T *n);
static Node_T *insert_tag_strct(Node_T *curr, Node_T *n);
static Node_T *end_tag_optopt(Node_T *curr);

static state_t do_state_initial( instance_data_t *data );
static state_t do_state_child( instance_data_t *data );
static state_t do_state_sibling( instance_data_t *data );
static state_t do_state_parent( instance_data_t *data );
static state_t do_state_back( instance_data_t *data );
static state_t do_state_end( instance_data_t *data );

static void print_opening(const char *node, int indent);
static void print_closing(const char *node, int indent);
static void print_indent(int indent);

state_func_t* const state_table[ NUM_STATES ] = {
	do_state_initial, do_state_child, do_state_sibling, do_state_parent,
	do_state_back, do_state_end
};

state_t run_state( state_t cur_state, instance_data_t *data ) {
	return state_table[ cur_state ]( data );
};



Node_T *Doc_tree_curr(T tr)
{
	assert(tr);

	return tr->curr;
}

T Doc_tree_insert(T tr, Node_T *n)
{
	Chunk_E cntn;

	assert(n && *n);

	if (tr == NULL) { 		// insert first node
		NEW(tr);
		
		tr->root = tr->curr = n;
	} else { 			// operate on tree
		cntn = Node_type(*n);
		if (cntn & C_CNTND) {
			tr->curr = insert_contents(tr->curr, n);
		} else if (cntn & C_TAGND) {
			tr->curr = insert_tag(tr->curr, n);
		} else { 		// default handling

		}
	}

	return tr;
}

T Doc_tree_end(T tr, Tag_E tf, const char *tend)
{
	assert(tr);
	assert(tend && *tend);

	if (strcmp(Node_name(*tr->curr), tend) == 0) {
		if (Node_has_parent(tr->curr)) {
			if (T_OPTNL & tf) {
				tr->curr = end_tag_optopt(tr->curr);
			} else {
				tr->curr = Node_parent(tr->curr);
			}
		} else if (Node_has_back(tr->curr)) { 		// top level
			tr->curr = Node_back(tr->curr);
		} else {
			; 		// root element
		}
	} else { 			// warning mal-formed document

	}

	return tr;
}

void Doc_tree_print (T tr) {
	state_t cur_state = STATE_INITIAL;
	instance_data_t data;

	data.indent = 0;
	data.n = tr->root;

	do {
		cur_state = run_state(cur_state , &data);
	} while ( cur_state != END );
}

static Node_T *insert_contents(Node_T *curr, Node_T *n)
{
	INSERT(curr, n);

	return curr; 			// contents returns parent aka. curr
}

static Node_T *insert_tag(Node_T *curr, Node_T *n)
{
	Node_T *ret = NULL;
	Tag_E tt = Node_tag_type(*n);

	if (tt & T_OPTNL) {
		ret = insert_tag_optnl(curr, n);
	} else if (tt & T_STRCT) {
		ret = insert_tag_strct(curr, n);
	} else if (tt & T_VOID) {
		ret = insert_tag_void(curr, n);
	} else { 			// default handling

	}

	assert(ret && *ret);

	return ret;
}

static Node_T *insert_tag_optnl(Node_T *curr, Node_T *n)
{
	if (Node_tag_type(*curr) & T_OPTNL) {
		if (!Node_has_parent(curr)) { 	// top level ~> no parent
			Node_add_sibling(curr, n);
			return n;
		} else {
			// close curr ~> make curr's parent curr
			curr = Node_parent(curr);
		}
	}

	INSERT(curr, n);

	return n; 		// same as strict
}

static Node_T *insert_tag_void(Node_T *curr, Node_T *n)
{
	INSERT(curr, n);

	return curr; 		// void returns parent, same as content
}

static Node_T *insert_tag_strct(Node_T *curr, Node_T *n)
{
	INSERT(curr, n);

	return n; 		// strict returns pointer to n
}

static Node_T *last_sibling(Node_T *n)
{
	while (Node_has_sibling(n)) {
		n = Node_sibling(n);
	}

	return n;
}

static Node_T *end_tag_optopt(Node_T *curr)
{
	Node_T *tmp = Node_back(curr);
	if (T_OPTNL == Node_tag_type(*tmp)) {
		return tmp;
	}

	return Node_parent(curr);
}

static state_t do_state_initial( instance_data_t *data )
{
	int indent = data->indent;
	Node_T *n = data->n;

	print_opening(Node_print(n), indent);

	if (Node_has_child(n)) {
		data->indent += 1;
		data->n = Node_child(n);
		return CHILD;
	}

	if (Node_has_sibling(n)) {
		data->n = Node_sibling(n);
		return SIBLING;
	}
}

static state_t do_state_child( instance_data_t *data )
{
	int indent = data->indent;
	Node_T *n = data->n;

	print_opening(Node_print(n), indent);

	if (Node_has_child(n)) {
		data->indent += 1;
		data->n = Node_child(n);
		return CHILD;
	}

	if (Node_has_sibling(n)) {
		data->n = Node_sibling(n);
		return SIBLING;
	}

	if (Node_has_parent(n)) {
		data->indent -= 1;
		data->n = Node_parent(n);
		return PARENT;
	}
}

static state_t do_state_sibling( instance_data_t *data )
{
	int indent = data->indent;
	Node_T *n = data->n;

	print_opening(Node_print(n), indent);

	if (Node_has_child(n)) {
		data->indent += 1;
		data->n = Node_child(n);
		return CHILD;
	}

	if (Node_has_sibling(n)) {
		data->n = Node_sibling(n);
		return SIBLING;
	}

	if (Node_has_parent(n)) {
		data->indent -= 1;
		data->n = Node_parent(n);
		return PARENT;
	}

	return END;
}

static state_t do_state_parent( instance_data_t *data )
{
	int indent = data->indent;
	Node_T *n = data->n;

	print_closing(Node_name(*n), indent);

	if (Node_has_sibling(n)) {
		data->n = Node_sibling(n);
		return SIBLING;
	}

	if (Node_has_parent(n)) {
		data->indent -= 1;
		data->n = Node_parent(n);
		return PARENT;
	}

	if (Node_has_back(n)) {
		data->indent -= 1;
		data->n = Node_back(n);
		return BACK;
	}

	return END;
}

static state_t do_state_back( instance_data_t *data )
{
	Node_T *n = data->n;

	if (Node_has_back(n)) {
		data->n = Node_back(n);
		return BACK;
	}

	return END;
}

static state_t do_state_end( instance_data_t *data )
{
	Node_T *n = data->n;

	print_closing(Node_name(*n), data->indent);
}

static void print_opening(const char *node, int indent)
{
	char *out = (char *) node;
	print_indent(indent);
	Fmt_fprint(stdout, "%s\n", out);

	FREE(out);
}

static void print_closing(const char *node, int indent)
{
	char *out = (char *) node;
	print_indent(indent);
	Fmt_fprint(stdout, "</%s>\n", node);
}

static void print_indent(int indent)
{
	int i = indent * INDENT_SIZE;

	while (i >= 0) {
		putc(' ', stdout);
		i--;
	}
}

