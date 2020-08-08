//
// Created by Spencer Dixon on 6/9/20.
//

#pragma once

#include <SD/Color.h>
#include <SD/String.h>
#include <SD/Types.h>

#include "Emulator/GUI/Bitmap.h"
#include "Emulator/GUI/Rect.h"
#include "Emulator/GUI/Texture.h"

// TODO: An API I've enjoyed in the past is the ability to set an offset and provide a block/function
// and every render call inside that block happens at the offset then the offset gets popped off
// when the function finishes execution. Consider adding something similar to my Renderer.

class Renderer {
public:
    virtual ~Renderer() {};
    virtual void init(const String& font_path) = 0;
    virtual void clear(const Color&) = 0;
    virtual void present() = 0;

    virtual void draw_text(const String&, const Point&) = 0;
    virtual void draw_rect(const Rect& rect, const Color& color) = 0;
    virtual void draw_texture(const Texture& tex, const Rect& rect) = 0;
    virtual void draw_texture(const Texture& tex, const Point& point) = 0;
    virtual void draw_texture_rotated(const Texture& tex, const Point& point, f64 angle) = 0;
    virtual void draw_partial_texture(const Texture& tex, const Rect& src, const Rect& dest) = 0;
};
