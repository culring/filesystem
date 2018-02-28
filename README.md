# filesystem

* [Introduction](#introduction)
* [How to use](#how-to-use)
* [Testing](#testing)
* [Implemention details](#implementation-details)

# Introduction

It is an implementation of a very simple file system. Basically, you can transfer files from your disk to the file system as well as in the opposite direction, trace a current memory map or have an access to some basic linux-like operations like rm or ls.

# How-to-use

You can create a new file system with following arguments:

```
create <path to filesystem> <number of bytes for filesystem>
```

If you want to open an existing one, change command for:

```
open <path to filesytem> 
```

Once program runs, you can do some stuff with a file system (keep each command/argument in a separate line):

- _copy_to_fs_ copies a file from your disk to the file system, i.e.:

```
copy_to_fs 
<name of file on our disk>
<name for a file in the file system>
```

- _copy_to_os_ copies a file from the file system to our disk, i.e.:

```
copy_to_os
<name of a file in the file system>
<name for new, copied file on our disk>
```

- _rm_ removes a file from the file system, i.e.:

```
rm
<name of file>
```

- _show_memory_map_ displays a current memory map of the file system

- _remove_fs_ removes a file system

- _ls_ just like the command in Linux - shows content of the catalogue

- _exit_ terminates the program
  
# Testing

You can also test the file system by running a script test1.sh which moves three types of files: binaries, images and plain text files to a created file system and then moves them back to your disk. In the end, checks if they are unchanged after those operations. To run that script put in the command line:

```bash
> chmod +x test1.sh
> ./test1.sh test	
```
	
After all, you can clean up temporary files created by script by typing:
	
```bash
> ./test1.sh clean
```

# Implementation-details
