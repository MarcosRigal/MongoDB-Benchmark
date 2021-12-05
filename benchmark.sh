#!/bin/bash
make clean
make
cd bin/
./main
cd ..
cd src/
python3 main.py