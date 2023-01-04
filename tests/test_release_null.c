#include "test.h"


void test_impl()
{
  hashtable_release(NULL);
}


int main()
{
  test_run(test_impl);
}

