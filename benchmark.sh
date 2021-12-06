#!/bin/bash
rm -Rf mongo*
mkdir reports/
make clean
make
cd bin/
./main
cd ..
cd src/
python3 main.py