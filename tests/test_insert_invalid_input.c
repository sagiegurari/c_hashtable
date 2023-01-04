#include "test.h"


void test_impl()
{
  bool done = hashtable_insert(NULL, "key", "value", NULL);

  assert_true(!done);

  struct HashTable *table = hashtable_new();

  done = hashtable_insert(table, NULL, "value", NULL);
  assert_true(!done);
  assert_true(hashtable_is_empty(table));

  hashtable_release(table);
}


int main()
{
  test_run(test_impl);
}

