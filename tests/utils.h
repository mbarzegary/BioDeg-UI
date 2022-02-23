#include <string>
#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

int run_command(string);
int run_ff(string, int);
bool mkdir(string);
bool rmdir(string);
bool file_exists(string);
string prepare_args(string, int, int, float, string, string, int, string);
