#! /bin/bash

# ./setup.sh 10.0.0.22 10.0.0.22 10.0.0.22 10.0.0.22 &
# ./setup.sh 10.0.0.22 10.0.0.22 10.0.0.22 10.0.0.21 > /dev/null &
# ./setup.sh 10.0.0.22 10.0.0.22 10.0.0.21 10.0.0.21 > /dev/null &
# ./setup.sh 10.0.0.22 10.0.0.21 10.0.0.21 10.0.0.21 > /dev/null &

./setup.sh 127.0.0.1 127.0.0.1 127.0.0.1 127.0.0.1 1 &
./setup.sh 127.0.0.1 127.0.0.1 127.0.0.1 127.0.0.1 2 > node2.txt  &
./setup.sh 127.0.0.1 127.0.0.1 127.0.0.1 127.0.0.1 3 > node3.txt   &
./setup.sh 127.0.0.1 127.0.0.1 127.0.0.1 127.0.0.1 4 > node4.txt   &
