//
// Created by Spencer Dixon on 6/9/20.
//

#include "SDLRenderer.h"
#include "Emulator/PPU.h"
#include <SD/LogStream.h>

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

    // TODO: Add API for setting title which can be used by ROMParser
    SDL_SetWindowTitle(m_window, "SpenceBoy");

    m_gb_screen = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        GB_WIN_WIDTH,
        GB_WIN_HEIGHT);
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

void SDLRenderer::draw_texture(const Texture& tex, const Rect& rect)
{
    auto dest = rect.to_sdl();

    SDL_RenderCopy(m_renderer, static_cast<SDL_Texture*>(tex.data()), NULL, &dest);
}

void SDLRenderer::draw_partial_texture(const Texture& tex, const Rect& src, const Rect& dest)
{
    auto src_rect = src.to_sdl();
    auto dest_rect = dest.to_sdl();
    SDL_RenderCopy(m_renderer, static_cast<SDL_Texture*>(tex.data()), &src_rect, &dest_rect);
}
