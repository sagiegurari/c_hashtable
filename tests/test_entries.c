#include "test.h"


static size_t _test_hash_function(char *key)
{
  assert_true(key != NULL);
  return(10000);
}


static void _test_assert_value(struct HashTableEntries entries, char *key, void *value, size_t size)
{
  assert_true(size > 0);

  for (size_t index = 0; index < size; index++)
  {
    if (!strcmp(entries.keys[index], key))
    {
      assert_string_equal(entries.values[index], (char *)value);
      return;
    }
  }

  test_fail();
}


void test_impl()
{
  struct HashTableEntries entries = hashtable_entries(NULL);

  assert_true(entries.keys == NULL);
  assert_true(entries.values == NULL);

  struct HashTableOptions options = { .table_size = 5, .chain_max_size = 3, .hash_function = _test_hash_function };
  struct HashTable        *table  = hashtable_new_with_options(options);

  bool                    done   = false;
  char                    *value = NULL;

  assert_true(hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 0);
  done = hashtable_insert(table, "key1", strdup("test1"), release_only_value);
  assert_true(done);
  assert_true(!hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 1);
  done = hashtable_insert(table, "key2", "test2", release_none);
  assert_true(done);
  assert_true(!hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 2);
  done = hashtable_insert(table, strdup("key3"), strdup("test3"), release_all);
  assert_true(done);
  assert_true(!hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 3);
  value = hashtable_get(table, "key1");
  assert_string_equal(value, "test1");
  value = hashtable_get(table, "key2");
  assert_string_equal(value, "test2");
  value = hashtable_get(table, "key3");
  assert_string_equal(value, "test3");

  entries = hashtable_entries(table);
  assert_true(entries.keys != NULL);
  assert_true(entries.values != NULL);
  _test_assert_value(entries, "key1", "test1", hashtable_size(table));
  _test_assert_value(entries, "key2", "test2", hashtable_size(table));
  _test_assert_value(entries, "key3", "test3", hashtable_size(table));
  free(entries.keys);
  free(entries.values);

  done = hashtable_insert(table, "key4", strdup("test4"), release_only_value);
  assert_true(!done);
  assert_true(!hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 3);

  value = hashtable_get(table, "key1");
  assert_string_equal(value, "test1");
  value = hashtable_get(table, "key2");
  assert_string_equal(value, "test2");
  value = hashtable_get(table, "key3");
  assert_string_equal(value, "test3");

  entries = hashtable_entries(table);
  assert_true(entries.keys != NULL);
  assert_true(entries.values != NULL);
  _test_assert_value(entries, "key1", "test1", hashtable_size(table));
  _test_assert_value(entries, "key2", "test2", hashtable_size(table));
  _test_assert_value(entries, "key3", "test3", hashtable_size(table));
  free(entries.keys);
  free(entries.values);

  done = hashtable_remove(table, "key3");
  assert_true(done);
  assert_true(!hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 2);

  entries = hashtable_entries(table);
  assert_true(entries.keys != NULL);
  assert_true(entries.values != NULL);
  _test_assert_value(entries, "key1", "test1", hashtable_size(table));
  _test_assert_value(entries, "key2", "test2", hashtable_size(table));
  free(entries.keys);
  free(entries.values);

  done = hashtable_insert(table, "key4", strdup("test4"), release_only_value);
  assert_true(done);
  assert_true(!hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 3);

  value = hashtable_get(table, "key1");
  assert_string_equal(value, "test1");
  value = hashtable_get(table, "key2");
  assert_string_equal(value, "test2");
  value = hashtable_get(table, "key4");
  assert_string_equal(value, "test4");

  entries = hashtable_entries(table);
  assert_true(entries.keys != NULL);
  assert_true(entries.values != NULL);
  _test_assert_value(entries, "key1", "test1", hashtable_size(table));
  _test_assert_value(entries, "key2", "test2", hashtable_size(table));
  _test_assert_value(entries, "key4", "test4", hashtable_size(table));
  free(entries.keys);
  free(entries.values);

  hashtable_release(table);
} /* test_impl */


int main()
{
  test_run(test_impl);
}

