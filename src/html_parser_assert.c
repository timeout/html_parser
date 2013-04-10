#include "html_parser_assert.h"

const Except_T Assert_Failed = {"Assertion failed"};

void (assert)(int e) {
	assert(e);
}
