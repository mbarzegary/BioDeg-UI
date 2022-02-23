#include "tests.h"

bool check_ff_exists()
{
  if (run_command("FreeFem++ check.edp") == 0)
    return true;
  else
    return false;
}

bool check_ff_parallel_exists(int mpisize)
{
  if (run_ff("check.edp", mpisize) == 0)
    return true;
  else
    return false;
}
