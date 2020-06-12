//
// Created by Spencer Dixon on 6/9/20.
//

#pragma once

#include <SD/String.h>
#include <SD/Types.h>
#include <SD/Color.h>

#include "Emulator/GUI/Bitmap.h"
#include "Emulator/GUI/Rect.h"
#include "Emulator/GUI/Texture.h"

//struct WindowSettings {
//    u16 width;
//    u16 height;
//    bool resizeable;
//    String title;
//};

// ACall: Should my renderer be project agnostic or is it okay to specialize it for my
// specific game boy rendering. I.E. instead of generic draw_XXX() calls I could have like
// draw_background(), draw_window(), draw_sprites(), etc.

class Renderer {
public:
    virtual ~Renderer() {};
    virtual void init() = 0;
    virtual void clear(const Color&) = 0;
    virtual void present() = 0;

    virtual void draw_texture(const Texture& tex, const Rect& rect) = 0;
    virtual void draw_texture(const Texture& tex, const Point& point) = 0;
    virtual void draw_texture_rotated(const Texture& tex, const Point& rect, f64 angle) = 0;
    virtual void draw_partial_texture(const Texture& tex, const Rect& src, const Rect& dest) = 0;
private:
};