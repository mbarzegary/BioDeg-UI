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

bool check_biodeg_diffusion(int mpisize)
{
  string dir_name = "diffuse";
  string output_name_1 = "output1.txt";
  string output_name_2 = "output2.txt";

  mkdir(dir_name);

  string args = prepare_args("../demo/screw.mesh", 2, 1, 0.1, dir_name, output_name_1, 0, "dummy");
  args = args + " -d_mg 0.05 -save_each 0.1";
  run_ff(args, mpisize);

  args = prepare_args("../demo/screw.mesh", 2, 1, 0.1, dir_name, output_name_2, 0, "dummy");
  args = args + " -d_mg 0.005 -save_each 0.1";
  run_ff(args, mpisize);

  bool result = true;

  if (!(file_exists(dir_name + "/" + output_name_1) && file_exists(dir_name + "/" + output_name_2)))
    result = false;

  float output1 =  get_last_mass_loss(dir_name + "/" + output_name_1);
  float output2 =  get_last_mass_loss(dir_name + "/" + output_name_2);

  if (output1 < 2 * output2)
    result = false;

  rmdir(dir_name);

  return result;
}
