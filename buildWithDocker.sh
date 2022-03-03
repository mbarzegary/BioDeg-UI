#!/bin/bash
# build the repo inside the docker container, used for CI builds and tests
cd /data
mkdir build
cd build
cmake ..
make
export PATH=$PATH:/usr/freefem/ff-petsc/r/bin/
make test
