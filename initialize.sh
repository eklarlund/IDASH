#! /bin/bash
source ${PWD}/paths.config

echo $MULTICHAIN_BIN

# add shell scripts to path
chmod +x connectToChain.sh
chmod +x listener.sh
chmod +x makeChain.sh
chmod +x nuke.sh
chmod +x setup.sh
chmod +x simulate.sh

#compile c++
g++ -o upload upload.cpp -std=c++11
g++ -o searchLog searchLog.cpp -std=c++11

