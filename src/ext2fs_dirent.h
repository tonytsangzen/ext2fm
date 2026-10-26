#ifndef __EXT2FS_DIRENT_H__
#define __EXT2FS_DIRENT_H__
#include <dirent.h>

#ifdef __cplusplus
extern "C" {
#endif

DIR *ext2fs_fdopendir(int fd);
DIR *ext2fs_opendir(const char *filename);
struct dirent *ext2fs_readdir(DIR *dirp);
int ext2fs_readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);
long ext2fs_telldir(DIR *dirp);
void ext2fs_seekdir(DIR *dirp, long loc);
void ext2fs_rewinddir(DIR *dirp);
int ext2fs_closedir(DIR *dirp);
int ext2fs_dirfd(DIR *dirp);

#ifdef __cplusplus
}
#endif
#endif /* __EXT2FS_DIRENT_H__ */
