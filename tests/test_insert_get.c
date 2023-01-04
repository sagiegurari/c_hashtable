#include "test.h"


void test_impl()
{
  struct HashTable *table = hashtable_new();

  bool             done   = false;
  char             *value = NULL;

  assert_true(hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 0);
  done = hashtable_insert(table, "key", strdup("test1"), release_only_value);
  assert_true(done);
  assert_true(!hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 1);
  value = hashtable_get(table, "key");
  assert_string_equal(value, "test1");
  done = hashtable_insert(table, "key", "test2", release_none);
  assert_true(done);
  assert_true(!hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 1);
  value = hashtable_get(table, "key");
  assert_string_equal(value, "test2");
  done = hashtable_insert(table, strdup("key2"), strdup("value"), release_all);
  assert_true(done);
  assert_true(!hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 2);
  value = hashtable_get(table, "key");
  assert_string_equal(value, "test2");
  value = hashtable_get(table, "key2");
  assert_string_equal(value, "value");

  hashtable_release(table);
}


int main()
{
  test_run(test_impl);
}

