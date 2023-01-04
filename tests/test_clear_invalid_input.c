#include "test.h"


void test_impl()
{
  hashtable_clear(NULL);
}


int main()
{
  test_run(test_impl);
}

