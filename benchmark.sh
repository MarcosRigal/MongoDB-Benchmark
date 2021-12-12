#!/bin/bash
sudo systemctl start mongod
rm -Rf mongo*
mkdir reports/
mkdir bin/
make clean
make
cd bin/
./main 1
./main 1
./main 1
./main 1
./main 1
./main 1
./main 1
./main 1
./main 1
./main 1
./main 2
./main 2
./main 2
./main 2
./main 2
./main 2
./main 2
./main 2
./main 2
./main 2
./main 3
./main 3
./main 3
./main 3
./main 3
./main 3
./main 3
./main 3
./main 3
./main 3
./main 4
./main 4
./main 4
./main 4
./main 4
./main 4
./main 4
./main 4
./main 4
./main 4
