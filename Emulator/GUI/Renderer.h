//
// Created by Spencer Dixon on 6/9/20.
//

#pragma once

#include <SD/String.h>
#include <SD/Types.h>
#include <SD/Color.h>

#include "Emulator/GUI/Bitmap.h"
#include "Emulator/GUI/Rect.h"

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

    // API Ideas:
    // void draw(Texture);
    // void fill_rect(Rect rect, Color color);
    virtual void draw_hardware() = 0;
    virtual void draw_bitmap(const Bitmap& bitmap, const Rect& rect) = 0;
private:
};