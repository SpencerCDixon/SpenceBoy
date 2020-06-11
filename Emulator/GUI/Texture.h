//
// Created by Spencer Dixon on 6/11/20.
//

#pragma once

#include "Emulator/GUI/Bitmap.h"
#include <SD/String.h>
#include <SD/Utility.h>

// TODO:
// 1. Create a new Texture class
// 2. Allow textures to be updated from Bitmaps
// 3. Let draw commands draw textures and not bitmaps (with rects)
// 4. Maybe a temporary texture could be created for bitmap draws?
// 5. Let PPU own it's own texture and the emulator will draw it where we want.

class Texture {
    SD_MAKE_NONCOPYABLE(Texture)
public:
    // TODO: This should not need to return a pointer but I couldn't figure out how to
    // avoid calling the copy-ctor which would then dtor the texture and remove from GPU.
    static Texture from_image(const String& image_path);
    Texture();
    ~Texture();
    Texture(const Texture&& tex);
    Texture& operator=(Texture&& other);

    void update_data(const Bitmap&);

    int width() { return m_width; }
    int width() const { return m_width; }
    int height() { return m_height; }
    int height() const { return m_height; }

    void* data() { return m_texture; }
    void* data() const { return m_texture; }

private:
    int m_height { 0 };
    int m_width { 0 };
    SDL_Texture* m_texture { nullptr };
};
