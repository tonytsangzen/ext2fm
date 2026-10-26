#include <ewoksys/timer.h>
#include <stdint.h>
#include <SDL2/SDL.h>

static struct timer_t* timer_list = NULL;

static uint32_t timer_id = 0;
static uint32_t timer_count = 0;

static uint32_t timer_handle(uint32_t interval, void* param) {
	if(param != NULL)
		 ((void (*)(void))param)();
	return interval;
}

uint32_t timer_set(uint32_t usec, timer_handle_t handle) {
	return SDL_AddTimer(usec/1000, timer_handle, (void*)handle);
}

void timer_remove(uint32_t id) {
	SDL_RemoveTimer(id);
}

#ifdef __cplusplus
}
#endif
