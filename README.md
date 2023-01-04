# hashtable

[![CI](https://github.com/sagiegurari/c_hashtable/workflows/CI/badge.svg?branch=master)](https://github.com/sagiegurari/c_hashtable/actions)
[![Release](https://img.shields.io/github/v/release/sagiegurari/c_hashtable)](https://github.com/sagiegurari/c_hashtable/releases)
[![license](https://img.shields.io/github/license/sagiegurari/c_hashtable)](https://github.com/sagiegurari/c_hashtable/blob/master/LICENSE)

> C Hash Table

* [Overview](#overview)
* [Usage](#usage)
* [Contributing](.github/CONTRIBUTING.md)
* [Release History](CHANGELOG.md)
* [License](#license)

<a name="overview"></a>
## Overview
This library provides a simple basic implementation of hashtables for C.

<a name="usage"></a>
## Usage

```c
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
```

## Contributing
See [contributing guide](.github/CONTRIBUTING.md)

<a name="history"></a>
## Release History

See [Changelog](CHANGELOG.md)

<a name="license"></a>
## License
Developed by Sagie Gur-Ari and licensed under the Apache 2 open source license.
