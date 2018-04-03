Unlike Softlink, a hardlink does not points to the file name of the originalfile. A hardlink points to the inode where the information of the original file is being store in the harddrive(meta data). 
A hardlink has the same permission, same length with the "linked file". 
Therefore, it is very hard to detect a hardlink since we do not know how to distinguish it with a normal file on the file system. Also ,When we fork(), different process will do fcopy for the same meta data, and could cause error.
