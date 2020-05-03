#include <SDL.h>
#include <SD/Types.h>
#include <SD/LogStream.h>
#include <iostream>

const u16 WIN_HEIGHT = 640;
const u16 WIN_WIDTH = 480;

void test_debug_utility() {
    dbg() << "unsigned examples: " << (u8)255 << " " << (u16)65535 << " " << (u32)32 << " " << (u64)64;
    dbg() << "signed examples: " << (s8)-127 << " " << (s16)(-200) << " " << (s32)(-2000) << " " << (s64)-4000;
    char const *message = "hello world";
    dbg() << "pointer to message: " << (void*)message << " and message is: " << message;
    dbg() << "floats: " << "3.14159265 (5 digits) == " << (f32)3.14159265 << " and f64: " << (f64)10.1234567;
}

int main() {
    test_debug_utility();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to init SDL");
        return -1;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_CreateWindowAndRenderer(WIN_HEIGHT, WIN_WIDTH, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    // Window Config
    SDL_SetWindowTitle(window, "Gameboy Color");
    SDL_RenderSetLogicalSize(renderer, WIN_HEIGHT, WIN_WIDTH);

    // Fill our buffer
    auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIN_HEIGHT,
                                     WIN_WIDTH);
    u32 myPixels[WIN_WIDTH * WIN_HEIGHT];
    for (int i = 0; i < WIN_HEIGHT * WIN_WIDTH; i++) {
        myPixels[i] = 0xffffffff;
    }

    SDL_UpdateTexture(texture, NULL, myPixels, WIN_WIDTH * sizeof(u32));

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN) {
                quit = true;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    return 0;
}
