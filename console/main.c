#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <signal.h>
#include "ext2fm.h"

static char _prompt[32] = {0};
static int _index = 0;
static char _command[256];

void handle_sigint(int sig) {
	printf("\n%s", _prompt);
	_index = 0;
	memset(_command, 0, sizeof(_command));
	fflush(stdout);
}

char* shell(void){
	int len = 0;
	while(1){
		printf("%s", _prompt);
		memset(_command, 0, sizeof(_command));
		_index = 0;
		while(1){
			char ch = getchar();
			_command[_index++] = ch;
			_command[_index] = '\0';
			if(ch == '\n'){
				if(_index > 1)
					return _command;
				else
					break;
			}
		}
	}
}

int main(int argc, char* argv[]){
	fs_t* mount = NULL;

	if(argc != 2){
		printf("ext2fm [dev]\n");
		return -1;
	}

	mount = e2fm_mount(argv[1], 1);
	if(mount == NULL){
		printf("mount faile!");
		return -1;
	}
	sprintf(_prompt, "%s>", basename(argv[1]));
	signal(SIGINT, handle_sigint);
	while (1)
	{
		fflush(stdout);
		char* cmd = shell();
		if(cmd != NULL){
			if(strncmp(cmd, "quit", 4) == 0 || strncmp(cmd, "exit", 4) == 0)
				break;
			int ret = e2fm_exec(cmd);
			if(ret != 0){
				 printf("Error %d\n", ret);
			}
		}
	}
	e2fm_unmount(mount);
	return 0;
}
