#!bin/bash

cd build
cmake ..
make
echo "\n------Build completed successfully. Running the test...------\n"
./xbyak_test