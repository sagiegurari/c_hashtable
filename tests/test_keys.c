#include "test.h"


static size_t _test_hash_function(char *key)
{
  assert_true(key != NULL);
  return(10000);
}


static void _test_assert_value(char **array, char *value, size_t size)
{
  assert_true(size > 0);

  for (size_t index = 0; index < size; index++)
  {
    if (!strcmp(array[index], value))
    {
      return;
    }
  }

  test_fail();
}


void test_impl()
{
  char **keys = hashtable_keys(NULL);

  assert_true(keys == NULL);

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

  keys = hashtable_keys(table);
  assert_true(keys != NULL);
  _test_assert_value(keys, "key1", hashtable_size(table));
  _test_assert_value(keys, "key2", hashtable_size(table));
  _test_assert_value(keys, "key3", hashtable_size(table));
  free(keys);

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

  keys = hashtable_keys(table);
  assert_true(keys != NULL);
  _test_assert_value(keys, "key1", hashtable_size(table));
  _test_assert_value(keys, "key2", hashtable_size(table));
  _test_assert_value(keys, "key3", hashtable_size(table));
  free(keys);

  done = hashtable_remove(table, "key3");
  assert_true(done);
  assert_true(!hashtable_is_empty(table));
  assert_num_equal(hashtable_size(table), 2);

  keys = hashtable_keys(table);
  assert_true(keys != NULL);
  _test_assert_value(keys, "key1", hashtable_size(table));
  _test_assert_value(keys, "key2", hashtable_size(table));
  free(keys);

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

  keys = hashtable_keys(table);
  assert_true(keys != NULL);
  _test_assert_value(keys, "key1", hashtable_size(table));
  _test_assert_value(keys, "key2", hashtable_size(table));
  _test_assert_value(keys, "key4", hashtable_size(table));
  free(keys);

  hashtable_release(table);
} /* test_impl */


int main()
{
  test_run(test_impl);
}

