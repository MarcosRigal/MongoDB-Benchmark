#!/bin/bash
#This script install mongodb, mongo compass & mongo conector for C++
#Author: MarcosRigal 28-11-2021
sudo apt update
sudo apt upgrade
sudo add-apt-repository universe
sudo apt-get install git
sudo apt-get install g++ -y
sudo apt install curl -y
sudo apt-get install gnupg -y
sudo apt --fix-broken install 
wget -qO - https://www.mongodb.org/static/pgp/server-5.0.asc | sudo apt-key add -
echo "deb [ arch=amd64,arm64 ] https://repo.mongodb.org/apt/ubuntu focal/mongodb-org/5.0 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-5.0.list
sudo apt-get update
sudo apt-get install -y mongodb-org -y
sudo apt --fix-broken install 
sudo systemctl daemon-reload
sudo systemctl start mongod
wget https://downloads.mongodb.com/compass/mongodb-compass_1.29.5_amd64.deb
sudo dpkg -i mongodb-compass_1.29.5_amd64.deb
sudo apt --fix-broken install 
sudo dpkg -i mongodb-compass_1.29.5_amd64.deb
sudo apt purge --auto-remove cmake
sudo apt --fix-broken install 
sudo apt update && sudo apt install -y software-properties-common lsb-release && sudo apt clean all
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
sudo apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"
sudo apt update
sudo apt install cmake -y
sudo apt-get install libmongoc-1.0-0 -y
sudo apt-get install libbson-1.0-0
sudo apt-get install cmake libssl-dev libsasl2-dev -y
wget https://github.com/mongodb/mongo-c-driver/releases/download/1.20.0/mongo-c-driver-1.20.0.tar.gz
tar xzf mongo-c-driver-1.20.0.tar.gz
cd mongo-c-driver-1.20.0
mkdir cmake-build
cd cmake-build
cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
cmake --build .
sudo cmake --build . --target install
cd
curl -OL https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.6.6/mongo-cxx-driver-r3.6.6.tar.gz
tar -xzf mongo-cxx-driver-r3.6.6.tar.gz
cd mongo-cxx-driver-r3.6.6/build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local/
sudo cmake --build . --target EP_mnmlstc_core
cmake --build .
sudo cmake --build . --target install
cmake .. -DBSONCXX_OUTPUT_BASENAME=custom_bsoncxx -DMONGOCXX_OUTPUT_BASENAME=custom_mongocxx
sudo apt install python3-pip
pip3 install matplotlib
