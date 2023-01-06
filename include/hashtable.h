#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <stddef.h>

#define HASHTABLE_DEFAULT_SIZE    1000

struct HashTable;

struct HashTableOptions
{
  // 0 for default size
  size_t table_size;
  // 0 for unlimited size
  size_t chain_max_size;
  // NULL for default implementation
  size_t (*hash_function)(char *);
};

struct HashTableEntries
{
  char **keys;
  void **values;
};

/**
 * Creates and returns a new hashtable with default size and implementation.
 * Once no longer needed, it must be released.
 */
struct HashTable *hashtable_new(void);

/**
 * Creates and returns a new hashtable.
 * Once no longer needed, it must be released.
 */
struct HashTable *hashtable_new_with_options(struct HashTableOptions);

/**
 * Frees the memory of the provided hashtable.
 */
void hashtable_release(struct HashTable *);

/**
 * Removes and frees all entries.
 */
void hashtable_clear(struct HashTable *);

/**
 * Returns the current entries count.
 */
size_t hashtable_size(struct HashTable *);

/**
 * Returns true if the current hashtable is empty.
 */
bool hashtable_is_empty(struct HashTable *);

/**
 * Inserts the provided value for the given key.
 * Once the value is removed or the table is released, it will call the optional release functions.
 * If the operation failed (invalid input, max size reached, ...), false will be returned.
 */
bool hashtable_insert(struct HashTable *, char * /* key */, void * /* value */, void (*release)(char *, void *));

/**
 * Returns the value based on the provided key from the hashtable.
 */
void *hashtable_get(struct HashTable *, char * /* key */);

/**
 * Removes and frees the entry for the given key (if found).
 * If an entry was found, true will be returned.
 */
bool hashtable_remove(struct HashTable *, char * /* key */);

/**
 * Returns all keys as an array.
 * The array (not values) must be released once done.
 */
char **hashtable_keys(struct HashTable *);

/**
 * Returns all key/value pairs as arrays.
 * The arrays (not values) must be released once done.
 */
struct HashTableEntries hashtable_entries(struct HashTable *);

#endif

