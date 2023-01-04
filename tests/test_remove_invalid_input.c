#include "test.h"


void test_impl()
{
  bool done = hashtable_remove(NULL, "key");

  assert_true(!done);

  struct HashTable *table = hashtable_new();

  done = hashtable_remove(table, NULL);
  assert_true(!done);

  hashtable_release(table);
}


int main()
{
  test_run(test_impl);
}

