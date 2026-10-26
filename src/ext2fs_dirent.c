
#include <dirent.h>
#include <ext2fs/ext2fs.h>


DIR *ext2fs_fdopendir(int fd){
	return NULL;
}

DIR *ext2fs_opendir(const char *filename){
	return NULL;
}

struct dirent *ext2fs_readdir(DIR *dirp){
	return NULL;
}

int ext2fs_readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result){
	return 0;
}

long ext2fs_telldir(DIR *dirp){
	return 0;
}

void ext2fs_seekdir(DIR *dirp, long loc){
	
}

void ext2fs_rewinddir(DIR *dirp){

}

int ext2fs_closedir(DIR *dirp){
	return 0;
}

int ext2fs_dirfd(DIR *dirp){
	return 0;
}

