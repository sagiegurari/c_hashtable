#include "hashtable.h"
#include "vector.h"
#include <stdlib.h>
#include <string.h>

struct HashTable
{
  struct Vector *values;
  size_t        chain_max_size;
  size_t        (*hash_function)(char *);
  size_t        count;
};

struct HashTableEntry
{
  char *key;
  void *value;
  void (*release)(char *, void *);
};

struct HashTableEntryReplaceContext
{
  bool                  done;
  struct HashTableEntry *entry;
};

// private functions
static size_t _hashtable_hash_key(struct HashTable *, char *);
static size_t _hashtable_hash_function(char *);
static void _hashtable_entry_release(struct HashTableEntry *);
static struct Vector *_hashtable_get_chain(struct HashTable *, char *, bool create);
static void *_hashtable_for_entry_in_chain(struct HashTable *, struct Vector *, char *, void *, void * (*callback)(struct HashTable *, struct Vector *, struct HashTableEntry *, size_t index, void *));
static void *_hashtable_get_entry_callback(struct HashTable *, struct Vector *, struct HashTableEntry *, size_t index, void *);
static void *_hashtable_replace_or_add_entry_callback(struct HashTable *, struct Vector *, struct HashTableEntry *, size_t index, void *);
static void *_hashtable_remove_entry_callback(struct HashTable *, struct Vector *, struct HashTableEntry *, size_t index, void *);
static bool _hashtable_release_entries(struct HashTable *);
static void _hashtable_noop(void *);

struct HashTable *hashtable_new()
{
  struct HashTableOptions options = { .table_size = 0, .chain_max_size = 0, .hash_function = NULL };

  return(hashtable_new_with_options(options));
}

struct HashTable *hashtable_new_with_options(struct HashTableOptions options)
{
  size_t table_size = options.table_size;

  if (!table_size)
  {
    table_size = HASHTABLE_DEFAULT_SIZE;
  }

  struct HashTable *table = malloc(sizeof(struct HashTable));
  table->values         = vector_new_with_options(table_size, false);
  table->chain_max_size = options.chain_max_size;
  table->hash_function  = options.hash_function;
  if (table->hash_function == NULL)
  {
    table->hash_function = _hashtable_hash_function;
  }
  table->count = 0;

  return(table);
}


void hashtable_release(struct HashTable *table)
{
  if (table == NULL)
  {
    return;
  }

  _hashtable_release_entries(table);
  vector_release(table->values);
  free(table);
}


void hashtable_clear(struct HashTable *table)
{
  if (_hashtable_release_entries(table))
  {
    vector_clear(table->values);
    table->count = 0;
  }
}


size_t hashtable_size(struct HashTable *table)
{
  if (table == NULL)
  {
    return(0);
  }

  return(table->count);
}


bool hashtable_is_empty(struct HashTable *table)
{
  size_t size = hashtable_size(table);

  return(!size);
}


bool hashtable_insert(struct HashTable *table, char *key, void *value, void (*release)(char *, void *))
{
  struct Vector *chain = _hashtable_get_chain(table, key, true /* create */);

  if (chain == NULL)
  {
    return(false);
  }

  struct HashTableEntry *entry = malloc(sizeof(struct HashTableEntry));
  entry->key     = key;
  entry->value   = value;
  entry->release = release;

  struct HashTableEntryReplaceContext context = { .done = false, .entry = entry };
  _hashtable_for_entry_in_chain(table, chain, key, &context, _hashtable_replace_or_add_entry_callback);

  if (!context.done)
  {
    _hashtable_entry_release(entry);
  }

  return(context.done);
} /* hashtable_insert */


void *hashtable_get(struct HashTable *table, char *key)
{
  struct Vector *chain = _hashtable_get_chain(table, key, false /* create */);

  if (chain == NULL)
  {
    return(NULL);
  }

  struct HashTableEntry *entry = _hashtable_for_entry_in_chain(table, chain, key, NULL, _hashtable_get_entry_callback);
  if (entry == NULL)
  {
    return(NULL);
  }

  return(entry->value);
}


bool hashtable_remove(struct HashTable *table, char *key)
{
  struct Vector *chain = _hashtable_get_chain(table, key, false /* create */);

  if (chain == NULL)
  {
    return(false);
  }

  bool done = false;
  _hashtable_for_entry_in_chain(table, chain, key, &done, _hashtable_remove_entry_callback);

  return(done);
}


static size_t _hashtable_hash_key(struct HashTable *table, char *key)
{
  if (table == NULL || table->hash_function == NULL || key == NULL)
  {
    return(0);
  }

  size_t capacity = vector_capacity(table->values);
  if (!capacity)
  {
    return(0);
  }

  size_t index = table->hash_function(key);

  return(index % capacity);
}


static size_t _hashtable_hash_function(char *key)
{
  size_t index = 0;

  for (int char_index = 0; key[char_index]; char_index++)
  {
    index = 31 * index + (size_t)key[char_index];
  }

  return(index);
}


static void _hashtable_entry_release(struct HashTableEntry *entry)
{
  if (entry == NULL)
  {
    return;
  }

  if (entry->release != NULL)
  {
    entry->release(entry->key, entry->value);
  }

  free(entry);
}

static struct Vector *_hashtable_get_chain(struct HashTable *table, char *key, bool create)
{
  if (table == NULL || key == NULL)
  {
    return(NULL);
  }

  size_t        index = _hashtable_hash_key(table, key);

  struct Vector *chain = vector_get(table->values, index);

  if (chain == NULL && create)
  {
    if (table->chain_max_size)
    {
      chain = vector_new_with_options(table->chain_max_size, false);
    }
    else
    {
      chain = vector_new();
    }

    vector_set(table->values, index, chain);
  }

  return(chain);
}


static void *_hashtable_for_entry_in_chain(struct HashTable *table, struct Vector *chain, char *key, void *context, void * (*callback)(struct HashTable *, struct Vector *, struct HashTableEntry *, size_t index, void *))
{
  if (table == NULL || chain == NULL || key == NULL)
  {
    return(NULL);
  }

  size_t chain_size = vector_size(chain);
  for (size_t index = 0; index < chain_size; index++)
  {
    struct HashTableEntry *entry = vector_get(chain, index);
    if (entry != NULL && !strcmp(entry->key, key))
    {
      return(callback(table, chain, entry, index, context));
    }
  }

  return(callback(table, chain, NULL, 0, context));
}


static void *_hashtable_get_entry_callback(struct HashTable *table, struct Vector *chain, struct HashTableEntry *entry, size_t index, void *context)
{
  _hashtable_noop(table);
  _hashtable_noop(chain);
  _hashtable_noop(&index);
  _hashtable_noop(context);

  return(entry);
}


static void *_hashtable_replace_or_add_entry_callback(struct HashTable *table, struct Vector *chain, struct HashTableEntry *entry, size_t index, void *context)
{
  if (table == NULL || chain == NULL || context == NULL)
  {
    return(NULL);
  }

  struct HashTableEntryReplaceContext *operation_context = (struct HashTableEntryReplaceContext *)context;
  if (entry == NULL)
  {
    // we hit max chain size
    size_t chain_size = vector_size(chain);
    if (table->chain_max_size && table->chain_max_size <= chain_size)
    {
      return(NULL);
    }

    operation_context->done = vector_push(chain, operation_context->entry);
    if (operation_context->done)
    {
      table->count = table->count + 1;
    }
  }
  else
  {
    _hashtable_entry_release(entry);
    void *old_entry = vector_set(chain, index, operation_context->entry);
    operation_context->done = old_entry != NULL;
  }

  return(NULL);
}


static void *_hashtable_remove_entry_callback(struct HashTable *table, struct Vector *chain, struct HashTableEntry *entry, size_t index, void *context)
{
  _hashtable_noop(table);

  bool *bool_ptr = (bool *)context;
  if (entry == NULL)
  {
    *bool_ptr = false;
  }
  else
  {
    _hashtable_entry_release(entry);
    vector_remove(chain, index);
    table->count = table->count - 1;
    *bool_ptr    = true;
  }

  return(NULL);
}


static bool _hashtable_release_entries(struct HashTable *table)
{
  if (table == NULL)
  {
    return(false);
  }

  if (table->values == NULL)
  {
    return(false);
  }

  size_t table_size = vector_size(table->values);
  for (size_t table_index = 0; table_index < table_size; table_index++)
  {
    struct Vector *chain = vector_get(table->values, table_index);

    if (chain != NULL)
    {
      size_t chain_size = vector_size(chain);
      for (size_t chain_index = 0; chain_index < chain_size; chain_index++)
      {
        struct HashTableEntry *entry = vector_get(chain, chain_index);
        _hashtable_entry_release(entry);
      }

      vector_release(chain);
    }
  }

  table->count = 0;

  return(true);
}


static void _hashtable_noop(void *ignored)
{
  if (ignored == NULL)
  {
    return;
  }
}

