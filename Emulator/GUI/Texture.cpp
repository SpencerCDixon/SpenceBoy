//
// Created by Spencer Dixon on 6/11/20.
//

#include <SD/Assertions.h>
#include <SD/LogStream.h>

#include "Emulator/GUI/SDLRenderer.h"
#include "Emulator/InternalSDL.h"
#include "Texture.h"

Texture Texture::from_image(const String& image_path)
{
    Texture tex;

    auto sdl_tex = IMG_LoadTexture(SDLRenderer::the().renderer(), image_path.characters());

    ASSERT(sdl_tex);
    SDL_QueryTexture(sdl_tex, NULL, NULL, &tex.m_width, &tex.m_height);
    tex.m_texture = sdl_tex;

    return tex;
}

Texture Texture::from_size(const Size& size, TextureUsage usage)
{
    Texture tex;

    auto sdl_tex = SDL_CreateTexture(
        SDLRenderer::the().renderer(),
        SDL_PIXELFORMAT_ARGB8888,
        usage == TextureUsage::Static ? SDL_TEXTUREACCESS_STATIC : SDL_TEXTUREACCESS_STREAMING,
        size.width,
        size.height);
    ASSERT(sdl_tex);

    tex.m_texture = sdl_tex;
    tex.m_height = size.height;
    tex.m_width = size.width;

    return tex;
}

Texture::Texture()
{
}

Texture::Texture(Texture&& other)
{
    m_texture = other.m_texture;
    m_height = other.m_height;
    m_width = other.m_width;
    other.m_texture = nullptr;
}

Texture& Texture::operator=(Texture&& other)
{
    if (this != &other) {
        if (m_texture)
            SDL_DestroyTexture(m_texture);

        m_width = other.width();
        m_height = other.height();
        m_texture = other.m_texture;
        other.m_texture = nullptr;
    }

    return *this;
}

Texture::~Texture()
{
    if (m_texture)
        SDL_DestroyTexture(m_texture);
}

void Texture::set_data(const Bitmap& bitmap)
{
    SDL_UpdateTexture(m_texture, NULL, bitmap.data(), bitmap.pitch());
}

void Texture::color(const Color& color)
{
    SDL_SetTextureColorMod(m_texture, color.r, color.g, color.b);
}
