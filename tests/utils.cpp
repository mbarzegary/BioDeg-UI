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

bool mkdir(string dir_path)
{
  return fs::create_directory(dir_path.c_str());
}

bool rmdir(string dir_path)
{
  return fs::remove_all(dir_path.c_str());
}

bool file_exists(string file_path)
{
  fs::path p(file_path);
  return fs::exists(p);
}

string prepare_args(string mesh_file, int label_scaffold, int label_medium, float final_time,
                    string output_dir, string output_name, int write_vtk, string vtk_output_name)
{
  return "../core/src/main.edp -import_mesh 1 -mesh_file \"" + mesh_file + "\""
       + " -label_scaffold " + to_string(label_scaffold)
       + " -label_medium " + to_string(label_medium)
       + " -text_output_file " + output_dir + "/" + output_name
       + " -write_vtk " + to_string(write_vtk)
       + " -vtk_output_name " + output_dir + "/" + vtk_output_name
       + " -solve_fluid 0 -final_time " + to_string(final_time)
       + " -save_last_state 0";
}