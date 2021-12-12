#!/bin/bash
sudo systemctl start mongod
rm -Rf mongo*
mkdir reports/
mkdir bin/
make clean
make
cd bin/
./main
cd ..
cd src/
python3 main.py