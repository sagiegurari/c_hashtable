#include "test.h"


void test_impl()
{
  struct HashTable *table = hashtable_new();

  bool             done   = false;
  char             *value = NULL;

  assert_true(hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 0);
  done = hashtable_remove(table, "badkey");
  assert_true(!done);
  assert_true(hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 0);

  done = hashtable_insert(table, "key", strdup("test1"), release_only_value);
  assert_true(done);
  assert_true(!hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 1);
  done = hashtable_remove(table, "key");
  assert_true(done);
  assert_true(hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 0);
  value = hashtable_get(table, "key");
  assert_true(value == NULL);

  hashtable_release(table);
}


int main()
{
  test_run(test_impl);
}

