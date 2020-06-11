//
// Created by Spencer Dixon on 6/9/20.
//

#pragma once

#include "Emulator/InternalSDL.h"
#include "Emulator/GUI/Renderer.h"

class SDLRenderer final : public Renderer {
public:
    static SDLRenderer& the();

    SDLRenderer();
    virtual ~SDLRenderer() ;
    virtual void init(RuntimeSettings) override;
    virtual void clear(const Color&) override;
    virtual void present() override;

    // API Ideas:
    // void draw(Texture);
    // void fill_rect(Rect rect, Color color);
    void draw_bitmap(const Bitmap& bitmap, const Rect& rect) override;
    void draw_hardware() override;

private:
    SDL_Window* m_window { nullptr };
    SDL_Renderer* m_renderer { nullptr };

    // 256x256 screen
    SDL_Texture* m_gb_screen { nullptr };
    // Hardware background
    SDL_Texture* m_hardware_background { nullptr };

    RuntimeSettings m_settings;
};
