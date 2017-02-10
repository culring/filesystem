#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include "fs.h"
using namespace std;

int main(int argc, char **argv){
    VirtualDisk vd;
    if(argc < 3){
        throw wrong_arguments();
    }
    else{
        if(strcmp(argv[1], "create") == 0){
            if(argc < 4) throw wrong_arguments();
            vd.create(argv[2], atoi(argv[3]));
        }
        else if(strcmp(argv[1], "open") == 0){
            fstream virtualDisk(argv[2], ios::binary | ios::in | ios::out);
            if(!virtualDisk.good()) throw virtual_disk_cannot_be_opened();
            virtualDisk.seekg(sizeof(unsigned));
            virtualDisk.read(reinterpret_cast<char *>(&vd), sizeof(vd));
            cout << "Pojemnosc dysku: " << vd.getSize() << "B" << endl;
            cout << "Wolne miejsce na dysku: " << vd.getFreeMemory() << endl;
            virtualDisk.close();
        }
        else throw wrong_arguments();
    }
    
    string input1, input2;
    while(true){
        cout << "> ";
        getline(cin, input1);
        if(input1 == (string)"copy_to_fs"){
            getline(cin, input1);
            getline(cin, input2);
            vd.copyFileToFS(input1.c_str(), input2.c_str());
        }
        else if(input1 == (string)"copy_to_os"){
            getline(cin, input1);
            getline(cin, input2);
            vd.copyFileToOS(input1.c_str(), input2.c_str());
        }
        else if(input1 == (string)"exit"){
            break;
        }
        else if(input1 == (string)"rm"){
            getline(cin, input1);
            vd.erase(input1.c_str());
        }
        else if(input1 == (string)"show_memory_map"){
            vd.showMemoryMap();
        }
        else if(input1 == (string)"ls"){
            vd.ls();
        }
        else if(input1 == (string)"remove_fs"){
            if(remove(argv[2])) throw removing_fs_failed();
            else return 0;
        }
        else throw wrong_arguments();
    }
    return 0;
}
