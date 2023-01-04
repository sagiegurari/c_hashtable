#include "test.h"


void test_impl()
{
  struct HashTable *table = hashtable_new();

  hashtable_insert(table, "constkey", "constvalue", release_none);
  hashtable_insert(table, "constkey1", "constvalue1", NULL);
  hashtable_insert(table, "constkey2", strdup("value"), release_only_value);
  hashtable_insert(table, strdup("key1"), "constvalue", release_only_key);
  hashtable_insert(table, strdup("key2"), strdup("value"), release_all);

  char *value = hashtable_get(table, "constkey");
  assert_string_equal(value, "constvalue");
  assert_true(!hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 5);

  hashtable_clear(table);

  assert_true(hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 0);
  value = hashtable_get(table, "constkey");
  assert_true(value == NULL);

  // just make sure nothing happens if called over and over again
  hashtable_clear(table);
  hashtable_clear(table);
  hashtable_clear(table);
  hashtable_clear(table);

  assert_true(hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 0);

  hashtable_release(table);
}


int main()
{
  test_run(test_impl);
}

