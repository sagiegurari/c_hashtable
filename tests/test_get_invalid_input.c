#include "test.h"


void test_impl()
{
  void *data = hashtable_get(NULL, "key");

  assert_true(data == NULL);

  struct HashTable *table = hashtable_new();

  data = hashtable_get(table, NULL);
  assert_true(data == NULL);

  hashtable_release(table);
}


int main()
{
  test_run(test_impl);
}

