SHELL:=/bin/zsh
TOP:=$(shell pwd)

CII_OBJECTS=html_parser_assert.o html_parser_atom.o html_parser_except.o \
	    	html_parser_fmt.o html_parser_mem.o html_parser_arith.o \
		html_parser_set.o html_parser_text.o html_parser_tester.o
PARSER_OBJECTS=html_parser_attribute_rep.o  html_parser_attribute_list.o \
		html_parser_tag_reader.o html_parser_attribute_reader.o \
		html_parser_document_node.o html_parser_document_stack.o \
	       	html_parser_document_tree.o html_parser_file_reader.o \
		html_parser_tag_lookup.o html_parser_document_builder.o \
		html_parser_document_search.o
CII=libcii.a
PARSER_LIB=libhtml_parser.a
P=html_parser

SRC_DIR=src
OBJ_DIR=obj
LIB_DIR=lib
TOOLS_DIR=tools
TESTS_DIR=tests

CC=/usr/bin/clang
CFLAGS=-g -Wall -O3 -std=c11 -Iinclude
LDLIBS=-lhtml_parser -lcii -lc
LDFLAGS=$(CFLAGS) -static -L$(LIB_DIR)

$(P): $(CII) $(PARSER_LIB) header

$(CII): $(CII_OBJECTS)
	for f in $(CII_OBJECTS); do ar -rcs $(LIB_DIR)/$@ $(OBJ_DIR)/$$f; done

$(PARSER_LIB): $(PARSER_OBJECTS)
	for f in $(PARSER_OBJECTS); do ar -rcs $(LIB_DIR)/$@ $(OBJ_DIR)/$$f; done

html_parser_assert.o: $(SRC_DIR)/html_parser_assert.c
	@mkdir -p $(LIB_DIR) $(OBJ_DIR) $(TOOLS_DIR) 	# nasty way of bootstrapping dirs
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_atom.o: $(SRC_DIR)/html_parser_atom.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_except.o: $(SRC_DIR)/html_parser_except.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_fmt.o: $(SRC_DIR)/html_parser_fmt.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_mem.o: $(SRC_DIR)/html_parser_mem.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_arith.o: $(SRC_DIR)/html_parser_arith.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_set.o: $(SRC_DIR)/html_parser_set.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_tester.o: $(SRC_DIR)/html_parser_tester.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_text.o: $(SRC_DIR)/html_parser_text.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_attribute_rep.o: $(SRC_DIR)/html_parser_attribute_rep.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_attribute_list.o: $(SRC_DIR)/html_parser_attribute_list.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_tag_reader.o: $(SRC_DIR)/html_parser_tag_reader.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_attribute_reader.o: $(SRC_DIR)/html_parser_attribute_reader.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_tag_lookup.o: $(SRC_DIR)/html_parser_tag_lookup.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_document_node.o: $(SRC_DIR)/html_parser_document_node.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_document_stack.o: $(SRC_DIR)/html_parser_document_stack.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_document_tree.o: $(SRC_DIR)/html_parser_document_tree.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_file_reader.o: $(SRC_DIR)/html_parser_file_reader.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_document_builder.o: $(SRC_DIR)/html_parser_document_builder.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

html_parser_document_search.o: $(SRC_DIR)/html_parser_document_search.c
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

tests: $(CII) $(PARSER_LIB)
	for f in $(PARSER_OBJECTS); do \
		$(CC) $(LDFLAGS) -o $(TESTS_DIR)/$${f%\.*}.out \
			$(TESTS_DIR)/$${f%\.*}_test.c $(LDLIBS); \
	done;

.PHONY: tests-run

tests-run: tests
	# run any tests
	for f in $(PARSER_OBJECTS); do ./$(TESTS_DIR)/$${f%\.*}.out; done

.PHONY: header

header:
	@echo "\033[31mWarning: clobbering $(P).h\033[0m"
	@echo "#ifndef __HTML_PARSER_H_INCLUDED__\n\
#define __HTML_PARSER_H_INCLUDED__\n\n\
#ifdef __cplusplus\n\
extern \"C\" {\n\
#endif\n" > $(P).h

	for f in include/*; do echo "#include \"$${f}\"" \
		>> $(P).h; done

	@echo "\n#ifdef __cplusplus\n\
}\n\
#endif\n\
#endif /* __HTML_PARSER_H_INCLUDED__ /* \n" >> $(P).h

.PHONY: clean

clean:
	@printf "\033[31mWarning: cleaning...\n\033[0m"
	@find tests -type f -not -name "*.c" -and -not -name "*.skel" -print -delete
	@find $(TOP) -type f -name "*.o" -print -delete
	@find $(TOP) -type f -name "*.a" -print -delete
	@find $(TOP) -type f -name "*.core" -print -delete
	@printf "\033[31mWarning: removing directories %s, %s and %s\n\033[0m" $(OBJ_DIR) $(LIB_DIR) $(TOOLS_DIR)
	@rm -rfv $(OBJ_DIR) $(LIB_DIR) $(TOOLS_DIR)
	@printf "\033[32mdone.\n\033[0m"
