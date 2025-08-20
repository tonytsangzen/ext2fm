typedef void* e2fs_t;

e2fs_t* e2fs_mount(char* name, int rw);
int e2fs_exec(e2fs_t *mount, char* cmd);
void e2fs_unmount(e2fs_t* mount);
