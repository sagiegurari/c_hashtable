#ifndef __TEST_H__
#define __TEST_H__

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

void test_run(void (*fn)());
void test_fail();
void assert_true(bool);

void assert_num_equal(size_t, size_t);
void assert_string_equal(char *, char *);

void release_only_key(char *, void *);
void release_only_value(char *, void *);
void release_all(char *, void *);
void release_none(char *, void *);

#endif

