# filesystem

It is a simple example of filesystem which is based on idea of one catalogue. You can add files from your disk and also     download them back, observe an actual memory map and display all the names of existing files.

Run fs with arguments '<create> <path_to_filesystem> <number_of_bytes_for_filesystem>' to create new file system. If you want to open an existing one just change arguments for '<open> <path_to_filesytem>'. Then program runs and you can call some functions which you can pass arguments (each one must be written in separate lines):
	
	- copy_to_fs - copies a file from our disk to the file system, i.e.:
	  copy_to_fs 
	  file.txt // name of file on our disk
	  file // name for a file in the file system

	- copy_to_os - copies a file from the file system to our disk, i.e.:
	   copy_to_fs
	   file // name of a file in the file system
	   file.txt // name for new, copied file on our disk

	- rm - removes a file from the file system, i.e.:
		rm
		file

	- show_memory_map - displays an actual memory map of the file system

	- remove_fs - removes the file system from our disk

	- ls - just like the command in Linux - shows content of the current catalogue

	- exit - exits the program.
  
You can also test the file system by running special script test1.sh which moves three types of files: binaries, images and plain text files to a created file system and then moves them back to our disk. In the end, checks if they are unchanged after those operations.
