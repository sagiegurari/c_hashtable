#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void release_key_and_value(char *key, void *value);


int main()
{
  bool   done   = false;
  size_t size   = 0;
  char   *value = NULL;

  // Create new hashtable with default implementation and size.
  // You can also provide your own hash implementation using the new with options function as follows:
  // struct HashTableOptions options = { .table_size = 5, .chain_max_size = 0, .hash_function = my_hash_function };
  // struct HashTable        *table  = hashtable_new_with_options(options);
  struct HashTable *table = hashtable_new();

  // add a new entry to the table.
  // The value is void* so you can put anything you need
  done  = hashtable_insert(table, "key", "value", NULL /* release function */);
  value = hashtable_get(table, "key");
  size  = hashtable_size(table);
  printf("Pushed new entry: %d value is: '%s' current size: %zu\n", done, value, size);

  // To release the entry key/value you can provide a release function
  done  = hashtable_insert(table, strdup("key2"), strdup("value2"), release_key_and_value);
  value = hashtable_get(table, "key2");
  size  = hashtable_size(table);
  printf("Pushed new entry: %d value is: '%s' current size: %zu\n", done, value, size);

  // remove the value (content will be freed via custom release function)
  hashtable_remove(table, "key2");
  printf("Removed 1 entry, is empty: %d\n", hashtable_is_empty(table));

  hashtable_clear(table);
  printf("Removed all entries, is empty: %d\n", hashtable_is_empty(table));

  // once we are done, we need to release the table and its contents
  hashtable_release(table);

  return(0);
} /* main */


static void release_key_and_value(char *key, void *value)
{
  free(key);
  free(value);
}

