#ifndef X_SDL_BACKEND_H
#define X_SDL_BACKEND_H

#include <SDL2/SDL.h>

typedef struct {
    void *data;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_PixelFormat *format;
    uint32_t *framebuffer;
    int width;
    int height;
}sdl_context_t;


sdl_context_t* sdl_open(const char* title, int x, int y, int w, int h, int flag);
void sdl_render(sdl_context_t* ctx);
void sdl_close(sdl_context_t *ctx);

#endif