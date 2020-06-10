//
// Created by Spencer Dixon on 6/9/20.
//

#pragma once

#include "Emulator/InternalSDL.h"
#include "Renderer.h"

class SDLRenderer final : public Renderer {
public:
    SDLRenderer();
    virtual ~SDLRenderer();
    virtual void init() override;
    virtual void clear() override;
    virtual void present() override;

    // API Ideas:
    // void draw(Texture);
    // void fill_rect(Rect rect, Color color);
    void draw_bitmap(const Bitmap& bitmap, const Rect& rect) override;

private:
    SDL_Window* m_window { nullptr };
    SDL_Renderer* m_renderer { nullptr };
    SDL_Texture* m_gb_screen { nullptr };
};
