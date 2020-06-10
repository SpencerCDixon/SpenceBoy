//
// Created by Spencer Dixon on 6/9/20.
//

#include "SDLRenderer.h"
#include "Emulator/PPU.h"

// TODO: Get these dynamically from window settings.
constexpr u16 WIN_HEIGHT = 600;
constexpr u16 WIN_WIDTH = 400;

SDLRenderer::SDLRenderer()
{

}

SDLRenderer::~SDLRenderer()
{
}

void SDLRenderer::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        error("unable to initialize SDL video subsystem");
    }

    if (SDL_CreateWindowAndRenderer(WIN_HEIGHT, WIN_WIDTH, 0, &m_window, &m_renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        error("creating window and renderer");
    }

    // TODO: Get title dynamically from window settings.
    // TODO: Add API for setting title which can be used by ROMParser
    SDL_SetWindowTitle(m_window, "SpenceBoy");
    SDL_SetWindowResizable(m_window, SDL_bool::SDL_TRUE);
    SDL_RenderSetLogicalSize(m_renderer, WIN_HEIGHT, WIN_WIDTH);

    m_gb_screen = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        GB_WIN_WIDTH,
        GB_WIN_HEIGHT);
}

void SDLRenderer::clear()
{
    // TODO: Add color back to the clear func?
//    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
    SDL_RenderClear(m_renderer);
}

void SDLRenderer::present()
{
    SDL_RenderPresent(m_renderer);
}

void SDLRenderer::draw_bitmap(const Bitmap& bitmap, const Rect&)
{
    // TODO: For now to ease in refactor this will just draw into the gameboy texture.
    // In the future, this will become more generic!
    SDL_Rect rect = { 0, 0, 256, 256 };
    SDL_UpdateTexture(m_gb_screen, &rect, bitmap.data(), bitmap.pitch());
    SDL_RenderCopy(m_renderer, m_gb_screen, &rect, &rect);
}
