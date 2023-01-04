#include "test.h"


void test_impl()
{
  struct HashTable *table = hashtable_new();

  hashtable_insert(table, "constkey", "constvalue", release_none);
  hashtable_insert(table, "constkey1", "constvalue1", NULL);
  hashtable_insert(table, "constkey2", strdup("value"), release_only_value);
  hashtable_insert(table, strdup("key1"), "constvalue", release_only_key);
  hashtable_insert(table, strdup("key2"), strdup("value"), release_all);

  hashtable_release(table);
}


int main()
{
  test_run(test_impl);
}

