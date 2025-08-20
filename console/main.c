#include <stdio.h>
#include <string.h>
#include "ext2fm.h"

int main(int argc, char* argv[]){
	char cmd[256];

	if(argc != 3){
		printf("e2tool dev [script file]\n");
		return -1;
	}

	fs_t* mount = e2fm_mount(argv[1], 1);
	if(mount == NULL){
		printf("mount faile!");
		return -1;
	}

	printf("run script %s\n", argv[2]);
	FILE* fp = fopen(argv[2], "r");
	
	while(fgets(cmd, sizeof(cmd), fp) != NULL){
		cmd[strlen(cmd) -1] = '\0';
		int ret = e2fm_exec(cmd);
		if(ret != 0){
			printf("%d\n", ret);
			break;
		}
	}

	fclose(fp);

	e2fm_unmount(mount);
	return 0;
}
