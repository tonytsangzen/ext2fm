typedef void* fs_t;

fs_t* e2fm_mount(char* name, int rw);
int e2fm_exec(char* cmd);
void e2fm_unmount(fs_t* mount);
