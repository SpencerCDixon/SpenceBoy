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
    virtual void init(RuntimeSettings) override;
    virtual void clear(const Color&) override;
    virtual void present() override;

    // API Ideas:
    // void draw(Texture);
    // void fill_rect(Rect rect, Color color);
    void draw_bitmap(const Bitmap& bitmap, const Rect& rect) override;
//    void draw_hardware() override;
    void draw_texture(const Texture& tex, const Rect& rect) override;
    void draw_partial_texture(const Texture& tex, const Rect& src, const Rect& dest) override;

    // Texture Creation
    SDL_Renderer* renderer()
    {
        ASSERT(m_renderer);
        return m_renderer;
    }

private:
    SDL_Window* m_window { nullptr };
    SDL_Renderer* m_renderer { nullptr };

    // 256x256 screen
    SDL_Texture* m_gb_screen { nullptr };

    RuntimeSettings m_settings;
};
