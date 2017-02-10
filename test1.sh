#!/bin/sh

if [ "$1" == "test" ]  
then
    g++ fs.cpp -o fs
    g++ bin.cpp -o bin
    ./fs create vd 150000000 < test1.in 
    chmod +x bin2
    echo $'\n----------BIN (ORIGINAL)----------'
    ./bin 
    echo "----------BIN2 (COPIED)----------"
    ./bin2 
    echo "----------file.txt (ORIGINAL)----------"
    cat file.txt
    echo "----------file.txt (COPIED)----------"
    cat file2.txt
fi

if [ "$1" == "clean" ]
then
    rm fs bin bin2 plik1.jpg plik2.jpg plik6.jpg file2.txt vd
fi
