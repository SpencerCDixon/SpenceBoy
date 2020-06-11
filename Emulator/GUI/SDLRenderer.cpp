//
// Created by Spencer Dixon on 6/9/20.
//

#include "SDLRenderer.h"
#include "Emulator/PPU.h"
#include <SD/LogStream.h>

// TODO: Get these dynamically from window settings.
constexpr u16 WIN_HEIGHT = 500;
constexpr u16 WIN_WIDTH = 600;

static SDLRenderer s_the;
SDLRenderer& SDLRenderer::the()
{
    return s_the;
}

SDLRenderer::SDLRenderer()
{
}

SDLRenderer::~SDLRenderer()
{
}

void SDLRenderer::init(RuntimeSettings settings)
{
    m_settings = settings;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        error("unable to initialize SDL video subsystem");
    }

    if (SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, 0, &m_window, &m_renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        error("creating window and renderer");
    }

    // TODO: Get title dynamically from window settings.
    // TODO: Add API for setting title which can be used by ROMParser
    SDL_SetWindowTitle(m_window, "SpenceBoy");
    //    SDL_SetWindowResizable(m_window, SDL_bool::SDL_TRUE);
    //    SDL_RenderSetLogicalSize(m_renderer, WIN_HEIGHT, WIN_WIDTH);

    m_gb_screen = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        GB_WIN_WIDTH,
        GB_WIN_HEIGHT);

    // Image dimensions: 286x468
    // TODO: SDL_QueryTexture(img, NULL, NULL, &w, &h); // get the width and height of the texture
    auto hardware_background_path = m_settings.assets_dir + "/SpenceBoy.png";
    m_hardware_background = IMG_LoadTexture(m_renderer, hardware_background_path.characters());
}

void SDLRenderer::clear(const Color& color)
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, 255);
    SDL_RenderClear(m_renderer);
}

void SDLRenderer::present()
{
    SDL_RenderPresent(m_renderer);
}

void SDLRenderer::draw_bitmap(const Bitmap& bitmap, const Rect& rect)
{
    auto dest = rect.to_sdl();
    SDL_UpdateTexture(m_gb_screen, NULL, bitmap.data(), bitmap.pitch());
    SDL_RenderCopy(m_renderer, m_gb_screen, NULL, &dest);
}

void SDLRenderer::draw_hardware()
{
    SDL_Rect dest = { 20, 20, 286, 468 };
    SDL_RenderCopy(m_renderer, m_hardware_background, NULL, &dest);
}

// TODO:
// 1. Create a new Texture class
// 2. Allow textures to be updated from Bitmaps
// 3. Let draw commands draw textures and not bitmaps
// 4. Maybe a temporary texture could be created for bitmap draws?
