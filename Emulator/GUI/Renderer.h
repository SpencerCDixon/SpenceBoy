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

class Renderer {
public:
    virtual ~Renderer() {};
    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void present() = 0;

    // API Ideas:
    // void draw(Texture);
    // void fill_rect(Rect rect, Color color);
    virtual void draw_bitmap(const Bitmap& bitmap, const Rect& rect) = 0;
private:
};