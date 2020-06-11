//
// Created by Spencer Dixon on 6/11/20.
//

#include <SD/Assertions.h>
#include <SD/LogStream.h>

#include "Texture.h"
#include "Emulator/InternalSDL.h"
#include "Emulator/GUI/SDLRenderer.h"

Texture Texture::from_image(const String& image_path)
{
    Texture tex;

    auto sdl_tex = IMG_LoadTexture(SDLRenderer::the().renderer(), image_path.characters());
    ASSERT(sdl_tex);
    SDL_QueryTexture(sdl_tex, NULL, NULL, &tex.m_height, &tex.m_width);
    tex.m_texture = sdl_tex;

    return tex;
}

Texture::Texture()
{
}

Texture::Texture(const Texture&& other)
{
    m_texture = other.m_texture;
    m_height = other.m_height;
    m_width = other.m_width;
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
    dbg() << "~Texture()";
    if (m_texture)
        SDL_DestroyTexture(m_texture);
}

void Texture::update_data(const Bitmap& bitmap)
{
    SDL_UpdateTexture(m_texture, NULL, bitmap.data(), bitmap.pitch());
}

