//
// Created by Spencer Dixon on 6/9/20.
//

#include "SDLRenderer.h"
#include "Emulator/PPU.h"
#include <SD/Assertions.h>
#include <SD/LogStream.h>

constexpr u16 EMULATOR_WINDOW_HEIGHT = 500;
constexpr u16 EMULATOR_WINDOW_WIDTH = 600;

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
    if (m_font) {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void SDLRenderer::init(const String& font_path)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        error("unable to initialize SDL video subsystem");
    }

    if (SDL_CreateWindowAndRenderer(EMULATOR_WINDOW_WIDTH, EMULATOR_WINDOW_HEIGHT, 0, &m_window, &m_renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        error("creating window and renderer");
    }

    // Image initialization
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize PNG loading: %s", SDL_GetError());
        error("initializing png loading");
    }

    // Font initialization
    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF: %s", TTF_GetError());
        error("initializing font loading");
    }

    m_font = TTF_OpenFont(font_path.characters(), 12);

    // TODO: Add API for setting title which can be used by ROMParser
    SDL_SetWindowTitle(m_window, "SpenceBoy");

    // TODO: Move background out of the renderer and into a Background class
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

void SDLRenderer::draw_rect(const Rect& rect, const Color& color)
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_Rect sdl_rect = rect;
    SDL_RenderFillRect(m_renderer, &sdl_rect);
}

void SDLRenderer::draw_texture(const Texture& tex, const Rect& rect)
{
    SDL_Rect dest = rect;
    SDL_RenderCopy(m_renderer, static_cast<SDL_Texture*>(tex.data()), NULL, &dest);
}

void SDLRenderer::draw_texture(const Texture& tex, const Point& point)
{
    SDL_Rect rect { point.x, point.y, tex.width(), tex.height() };
    SDL_RenderCopy(m_renderer, static_cast<SDL_Texture*>(tex.data()), NULL, &rect);
}

void SDLRenderer::draw_texture_rotated(const Texture& tex, const Point& point, f64 angle)
{
    SDL_Rect rect { point.x, point.y, tex.width(), tex.height() };
    SDL_RenderCopyEx(m_renderer, static_cast<SDL_Texture*>(tex.data()), NULL, &rect, angle, NULL, SDL_FLIP_NONE);
}

void SDLRenderer::draw_partial_texture(const Texture& tex, const Rect& src, const Rect& dest)
{
    SDL_Rect src_rect = src;
    SDL_Rect dest_rect = dest;
    SDL_RenderCopy(m_renderer, static_cast<SDL_Texture*>(tex.data()), &src_rect, &dest_rect);
}

// todo since width/height are derived maybe this should take a point
void SDLRenderer::draw_text(const String& text, const Point& dest)
{
    if (m_last_text)
        SDL_DestroyTexture(m_last_text);

    ASSERT(m_font);
    SDL_Color text_color = { 125, 125, 125, 255 };
    SDL_Surface* text_surface = TTF_RenderText_Solid(m_font, text.characters(), text_color);

    ASSERT(text_surface);
    m_last_text = SDL_CreateTextureFromSurface(m_renderer, text_surface);

    ASSERT(m_last_text);
    SDL_Rect rect = { dest.x, dest.y, text_surface->w, text_surface->h };

    SDL_FreeSurface(text_surface);

    SDL_RenderCopy(m_renderer, m_last_text, NULL, &rect);
}
