import os
import sys

if (len(sys.argv) != 4): # go for running the default case
    os.system("paraview --script=without_fluid.py")
else: # write the arguments to the config file and run paraview
    file_name = sys.argv[1]
    array_name = sys.argv[2]
    colorbar_title = sys.argv[3]

    with open("config.txt", "w") as f:
        f.write("{0}\n".format(file_name))
        f.write("{0}\n".format(array_name))
        f.write("{0}\n".format(colorbar_title))

    os.system("paraview --script=without_fluid.py")
    
