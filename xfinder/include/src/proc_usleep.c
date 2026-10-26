#include <stdint.h>
#include <SDL2/SDL.h>

int proc_usleep(uint32_t usec){
	SDL_Delay(usec/1000);
	return 0;
}
