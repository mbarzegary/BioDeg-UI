// this file is not part of the testing module and never gets compiled
// it's used just to check the tests before putting them into google framewrok
// compile with: g++ main.cpp tests.cpp utils.cpp -std=c++17 -lstdc++fs

#include "tests.h"

int main()
{
  cout << check_ff_exists() << endl;
  cout << check_ff_parallel_exists(1) << endl;
  cout << check_ff_parallel_exists(3) << endl;

  return 0;
}
