//
// Created by Spencer Dixon on 6/9/20.
//

#pragma once

#include "Emulator/GUI/Renderer.h"
#include "Emulator/InternalSDL.h"
#include <SD/Assertions.h>

class SDLRenderer final : public Renderer {
public:
    static SDLRenderer& the();

    SDLRenderer();
    virtual ~SDLRenderer();
    virtual void init(const String& font_path) override;
    virtual void clear(const Color&) override;
    virtual void present() override;

    void draw_text(const String&, const Point&) override;
    void draw_rect(const Rect& rect, const Color& color) override;
    void draw_texture(const Texture& tex, const Rect& rect) override;
    void draw_texture(const Texture& tex, const Point& point) override;
    void draw_texture_rotated(const Texture& tex, const Point& rect, f64 angle) override;
    void draw_partial_texture(const Texture& tex, const Rect& src, const Rect& dest) override;

    SDL_Renderer* renderer()
    {
        ASSERT(m_renderer);
        return m_renderer;
    }

private:
    SDL_Window* m_window { nullptr };
    SDL_Renderer* m_renderer { nullptr };
    TTF_Font* m_font { nullptr };

    SDL_Texture* m_gb_screen { nullptr }; // 256x256 screen
    SDL_Texture* m_last_text { nullptr }; // font rendering
};
