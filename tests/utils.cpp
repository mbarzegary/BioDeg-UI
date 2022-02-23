#include "utils.h"

int run_command(string command)
{
  cout << "Executing: " << command << endl;
  return system(command.c_str());
}

int run_ff(string args, int mpisize)
{
  string command = "mpiexec -n " + to_string(mpisize) + " FreeFem++-mpi -v 0 " + args;
  return run_command(command);
}
