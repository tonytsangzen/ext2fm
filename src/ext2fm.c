#include "e2tools.h"
#include "ext2fm.h"
#include "util.h"
#include "e2tool-e2ls.h"

typedef struct {
	char dev[256];
	char alias[64];
	ext2_filsys fs;
	ext2_ino_t root;
}e2fs_t;

static e2fs_t *mount_list[20] = {NULL};

static int mount_list_add(e2fs_t *mount){
	for(int i = 0; i < sizeof(mount_list)/sizeof(fs_t*); i++){
		if(mount_list[i] == NULL){
			mount_list[i] = mount;
			return 0;
		}
	}
	return -1;
}

static int mount_list_del(e2fs_t *mount){
	for(int i = 0; i < sizeof(mount_list)/sizeof(fs_t*); i++){
        if(mount_list[i] == mount){
            mount_list[i] = NULL;
            return 0;
        }
    }
    return -1;
}

long get_entry_by_name(char *name, ext2_filsys *fs,  ext2_ino_t *root){
    for(int i = 0; i < sizeof(mount_list)/sizeof(fs_t*); i++){
        if(mount_list[i] != NULL && (strlen(name) == 0 || strcmp(name, mount_list[i]->dev) == 0 || strcmp(name, mount_list[i]->alias))){
			*fs = mount_list[i]->fs;
			*root = mount_list[i]->root;
			return 0;
        }
    }
    return -1;
}

struct cmd_element {
  const char *name;
  const char *alias;
  int (*main_func)(int argc, char *argv[]);
};

struct cmd_element cmdmap[] = {
    {"ls", "e2ls", main_e2ls},
    {"ln", "e2ln", main_e2ln},
    {"cp", "e2cp", main_e2cp},
    {"mv", "e2mv", main_e2mv},
    {"mkdir", "e2mkdir", main_e2mkdir},
    {"rm", "e2rm", main_e2rm},
};

static int parse(char*cmd, char* argc[], int max){
	int start = 1;
	int count = 0;

	while(*cmd != '\0'){
		if(*cmd == ' ' || *cmd == '\t'){
			start = 1;
			*cmd ='\0';
		}else if(*cmd == '\n'){
			*cmd = '\0';
			break;
		}else if(start){
			argc[count]	 = cmd;
			start = 0;
			count++;
			if(count >= max)
				break;
		}
		cmd++;
	}

	argc[count] = NULL;
	return count;
}

fs_t* e2fm_mount(char* dev, int rw){
	int retval;
	int closeval;

	e2fs_t *mount = malloc(sizeof(fs_t));
	if(!mount){
		fprintf(stderr, "malloc error!");
		return NULL;
	}

	if ((retval = ext2fs_open(dev, ((rw) ? EXT2_FLAG_RW : 0) |IO_FLAG_EXCLUSIVE, 0, 0,unix_io_manager, &mount->fs))){
  	    fprintf(stderr, "%s: %s\n", error_message(retval), dev);
		free(mount);
  	    return NULL;
  	}

	if((retval = io_channel_set_options(mount->fs->io, "cache=on&cache_blocks=1024"))){
		fprintf(stderr, "io_channel_set_options error: %s\n", error_message(retval));
		return NULL;
	}

	if ((retval = ext2fs_read_inode_bitmap(mount->fs))){
  	    fprintf(stderr, "%s: %s\n", error_message(retval), dev);
  	    if ((retval = close_filesystem(mount->fs)))
  			  fputs(error_message(retval), stderr);
        free(mount);
  	    return NULL;
  	}

  	if ((retval = ext2fs_read_block_bitmap(mount->fs))){
		  fprintf(stderr, "%s: %s\n", error_message(retval), dev);
  		if ((retval = close_filesystem(mount->fs)))
			  fputs(error_message(retval), stderr);
		free(mount);
  		return NULL;
  	}

	mount->root = EXT2_ROOT_INO;
	strncpy(mount->dev, dev, sizeof(mount->dev));
	mount_list_add(mount);
	return (fs_t*)mount;
}

int e2fm_exec(char* cmd)
{
 	char* argv[20] = {0};
 	char temp[1024] = {0};

 	strncpy(temp, cmd, sizeof(temp) - 1);
	int argc = parse(temp, argv, sizeof(argv)/sizeof(char*));

	initialize_ext2_error_table();
 	for (unsigned int i=0; i < sizeof(cmdmap)/sizeof(struct cmd_element); ++i)
 	{
 	  	if (strcmp(argv[0], cmdmap[i].name) == 0 || strcmp(argv[0], cmdmap[i].alias) == 0)
 		{
 			return cmdmap[i].main_func(argc, argv);
 	    }
 	}

 	fprintf(stderr, "e2tools command not implemented\n");
 	return -2;
}

void e2fm_unmount(fs_t* fs){
	if(fs == NULL)
		return;

	e2fs_t *mount = (e2fs_t*)fs;

	ext2fs_flush(mount->fs);
	mount_list_del(mount);
	close_filesystem(mount->fs);
	free(mount);
}
