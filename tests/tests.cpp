#include "tests.h"

bool check_ff_exists()
{
  if (run_command("FreeFem++ ../tests/check.edp") == 0)
    return true;
  else
    return false;
}

bool check_ff_parallel_exists(int mpisize)
{
  if (run_ff("../tests/check.edp", mpisize) == 0)
    return true;
  else
    return false;
}

bool check_biodeg_write(int mpisize)
{
  string dir_name = "write";
  string output_name = "output.txt";
  string vtk_name = "out";

  mkdir(dir_name);

  string args = prepare_args("../demo/screw.mesh", 2, 1, 0.025, dir_name, output_name, 1, vtk_name);
  run_ff(args, mpisize);

  bool result = file_exists(dir_name + "/" + output_name)
              & file_exists(dir_name + "/" + vtk_name + "_" + to_string(mpisize) + ".pvd");

  rmdir(dir_name);

  return result;
}