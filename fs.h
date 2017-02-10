#include <exception>
#include <iostream>
using namespace std;

#define PRINT_MAP(x, y, z) cout << x << " - " << x+y-1 << ": " << z << ", " << y << "B" << endl

class wrong_arguments : public std::exception{
    public:
        const char * what() const throw() { return "Wrong arguments passed"; }
};

class virtual_disk_cannot_be_opened : public std::exception{
    public:
        const char * what() const throw() { return "The virtual disk cannot be opened"; }
};

class file_cannot_be_opened : public std::exception{
    public:
        const char * what() const throw() { return "The file cannot be opened"; }
};

class file_not_found : public std::exception{
    public:
        const char * what() const throw() { return "The file is not in the file system"; }
};

class not_enough_memory : public std::exception{
    public:
        const char * what() const throw() { return "The disk is too small"; }
};

class removing_fs_failed : public std::exception{
    public:
        const char * what() const throw() { return "Failure appeared in removing the file system"; }
};

class VirtualDisk{
    private:
        const static unsigned MAX_FILES = 10;
        const static unsigned MAX_FILENAME = 64;
        //size of filesystem
        unsigned size;
        //size of free memory in bytes
        unsigned freeMemory;
        //number of byte after which we will continue writing
        unsigned begin;
        //how many files were saved on disk so far
        unsigned filesCounter;
        //directory of file system
        char dir[MAX_FILENAME];
        
        //each file is given a unique key
        //which is used to access different tables
        struct Catalogue{
            char filenames[MAX_FILES][MAX_FILENAME];
        } catalogue;
        
        struct Inode{
            //address of the beginning of file (in bytes from the very beginning of file system)
            unsigned begin;
            //size of file
            unsigned size;
        } inodes[MAX_FILES];
        
        //deletes file of number 'nr' updating inodes array
        void erase(unsigned nr){
            freeMemory += inodes[nr].size;
            begin -= inodes[nr].size;
            
            fstream vd(dir, ios::binary | ios::in | ios::out);
            char buffer[1024];
            vd.seekp(inodes[nr].begin, vd.beg);
            unsigned bytesToWrite;
            for(int i = nr+1; i < filesCounter; ++i){
                bytesToWrite = inodes[i].size;
                vd.seekg(inodes[i].begin, vd.beg);
                while(bytesToWrite){
                    vd.read(buffer, (bytesToWrite >= 1024 ? 1024 : bytesToWrite));
                    bytesToWrite -= vd.gcount();
                    vd.write(buffer, vd.gcount());
                }
            }
            
            unsigned offset = inodes[nr].size;
            for(int i = nr; i < filesCounter-1; ++i){
                strcpy(catalogue.filenames[i], catalogue.filenames[i+1]);
                inodes[i] = inodes[i+1];
                inodes[i].begin -= offset;
            }
            --filesCounter;
            
            update();
        }
    
    public:
        //creates filesystem of given directory (on our disk)
        //and with given size
        void create(const char *directory, unsigned fsSize){
            size = fsSize;
            freeMemory = size - (sizeof(unsigned) + sizeof(*this));

            begin = sizeof(unsigned) + sizeof(*this);
            filesCounter = 0;
            if(fsSize < sizeof(unsigned) + sizeof(*this)) throw not_enough_memory();
            
            fstream virtualDisk;
            strcpy(dir, directory);
            
            virtualDisk.open(dir, ios::binary | ios::out);
            if(!virtualDisk.good()) throw virtual_disk_cannot_be_opened();
            
            virtualDisk.write(reinterpret_cast<const char *>(&size), sizeof(size));
            virtualDisk.write(reinterpret_cast<const char*>(this), sizeof(*this));
            virtualDisk.seekp(size-sizeof(char));
            char tmp = '\0';
            virtualDisk.write(&tmp, 1);
            virtualDisk.close();
        }
        
        //returns size of filesystem
        int getSize(){ return size; }
        
        //returns amount of not used space in filesystem
        int getFreeMemory(){ return freeMemory; }
        
        //returns number of created files
        int getFilesCounter(){ return filesCounter; }
        
        //returns id of seeking file
        //or -1 - in case the file isn't found
        int doesFileExist(const char *name){
            for(int i = 0; i < filesCounter; ++i)
                if(strcmp(name, catalogue.filenames[i]) == 0) return i;
            return -1;
        }
        
        //removes from fs a file with given name
        void erase(const char *name){
            unsigned nr;
            if( (nr = doesFileExist(name)) == -1) throw file_not_found();
            erase(nr);
        }
    
        //copies a file from our disk to filesystem
        void copyFileToFS(const char *src, const char *name){
            //checking if we can store information about another file
            if(filesCounter >= MAX_FILENAME) throw not_enough_memory();

            //opening the file
            fstream file(src, ios::binary | ios::in);
            if(!file) file_cannot_be_opened();
            
            //checking if we have enough space
            file.seekg(0, file.end);
            unsigned filesize;
            if( (filesize = file.tellg()) > freeMemory) throw not_enough_memory();
            file.seekg(0, file.beg);
            freeMemory -= filesize;
            
            //checking if such file already exists
            int nr;
            if( (nr = doesFileExist(name)) != -1) erase(nr);
            strcpy(catalogue.filenames[filesCounter], name);

            //writing the file to fs
            fstream vd(dir, ios::binary | ios::in | ios::out);
            vd.seekp(begin);
            char buffer[1024];
            while(file){
                file.read(buffer, 1024);
                vd.write(buffer, file.gcount());
            }
            inodes[filesCounter].begin = begin;
            inodes[filesCounter++].size = filesize;
            begin += filesize;
           
            file.close();
            vd.close();
            
            update();
        }
        
        //copies a file from filesystem to our disk
        void copyFileToOS(const char *filename, const char *dst){
            int nr;
            //checking if a file exists in fs
            if( (nr = doesFileExist(filename)) == -1) throw file_cannot_be_opened();
            
            fstream file(dst, ios::binary | ios::out);
            if(!file.good()) throw file_cannot_be_opened();
            fstream vd(dir, ios::binary | ios::in);
            vd.seekg(inodes[nr].begin);
            char buffer[1024];
            unsigned bytesToRead = inodes[nr].size;
            while(bytesToRead){
                if(bytesToRead < 1024)
                    vd.read(buffer, bytesToRead);
                else vd.read(buffer, 1024);
                file.write(buffer, vd.gcount());
                bytesToRead -= vd.gcount();
            }
            
            file.close();
            vd.close();
        }
        
        //writes on disk information about current state of filesystem
        //so we can launch it next time from the same place
        void update(){
            fstream vd(dir, ios::binary | ios::in | ios::out);
            vd.seekp(sizeof(unsigned));
            vd.write(reinterpret_cast<char *>(this), sizeof(*this));
            vd.close();
        }
        
        //displays actual memory map of filesystem
        void showMemoryMap(){
            if(!filesCounter)
                PRINT_MAP(0, begin, "special files");
            else
                PRINT_MAP(0, inodes[0].begin, "special files");
            for(int i = 0; i < filesCounter; ++i)
                PRINT_MAP(inodes[i].begin, inodes[i].size, "data (used)");
            if(freeMemory){
                if(filesCounter) PRINT_MAP(inodes[filesCounter-1].begin+inodes[filesCounter-1].size, freeMemory, "data (free)");
                else PRINT_MAP(begin, freeMemory, "data (free)");
            }
        }
        
        //displays all the files in the catalogue
        void ls(){
            if(!filesCounter)
                cout << "Catalogue is empty" << endl;
            else
                for(int i = 0; i < filesCounter; ++i)
                    cout << catalogue.filenames[i] << " " << inodes[i].size << "B" << endl;
        }
};
