//
// Created by Spencer Dixon on 6/11/20.
//

#pragma once

#include "Emulator/GUI/Bitmap.h"
#include <SD/String.h>
#include <SD/Utility.h>

enum class TextureUsage {
    Static,
    Streaming,
};

class Texture {
    SD_MAKE_NONCOPYABLE(Texture)
public:
    static Texture from_image(const String& image_path);
    static Texture from_size(const Size&, TextureUsage usage);

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
