//
// Created by Spencer Dixon on 6/1/20.
//

#include "Joypad.h"

Joypad::Joypad() : m_mode(JoypadReadMode::None)
{
}

u8 Joypad::read([[maybe_unused]] u16  address)
{
    // We only have 1 address being used for Joypad related logic so we don't need to use the address
    // to return the bitmask.


    return 0;
}

void Joypad::write([[maybe_unused]] u16 address, u8 value)
{
    // 0010 0000 - Read DPad
    // 0001 0000 - Read Buttons
    // 0011 0000 - None
    if ((value & 0x20) && !(value & 0x10)) {
        set_mode(JoypadReadMode::DPad);
    } else if ((value & 0x10) && !(value & 0x20)) {
        set_mode(JoypadReadMode::Button);
    } else {
        set_mode(JoypadReadMode::None);
    }
}

void Joypad::set_key_state(const Key& key, bool is_down)
{
    m_keys[static_cast<int>(key)] = is_down;
}

const LogStream& operator<<(const LogStream& stream, const Joypad& input)
{
    stream << "Input: " << input.to_bit_mask();
    return stream;
}

//
// InputDebugWindow
//

static SDL_Texture* load_texture_from_bmp(SDL_Renderer* renderer, const char* file_path)
{
    auto surface = SDL_LoadBMP(file_path);
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_LoadBMP() failed: %s", SDL_GetError());
        error("loading bitmap");
    }
    auto tex = SDL_CreateTextureFromSurface(renderer, surface);
    if (!tex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateTextureFromSurface() failed: %s", SDL_GetError());
        error("creating texture from bitmap");
    }
    SDL_FreeSurface(surface);
    return tex;
}

InputDebugWindow::InputDebugWindow(SDL_Renderer* renderer)
    : m_renderer(renderer)

{
    // FIXME: Does SDL have rotation? Seems silly to duplicate all these textures.
    m_right_tex = load_texture_from_bmp(renderer, "../Assets/input-bmps/right.bmp");
    m_left_tex = load_texture_from_bmp(renderer, "../Assets/input-bmps/left.bmp");
    m_down_tex = load_texture_from_bmp(renderer, "../Assets/input-bmps/down.bmp");
    m_up_tex = load_texture_from_bmp(renderer, "../Assets/input-bmps/up.bmp");
    m_a_tex = load_texture_from_bmp(renderer, "../Assets/input-bmps/a.bmp");
    m_b_tex = load_texture_from_bmp(renderer, "../Assets/input-bmps/b.bmp");
    m_start_tex = load_texture_from_bmp(renderer, "../Assets/input-bmps/start.bmp");
    m_select_tex = load_texture_from_bmp(renderer, "../Assets/input-bmps/select.bmp");
}

InputDebugWindow::~InputDebugWindow()
{
    if (m_right_tex)
        SDL_DestroyTexture(m_right_tex);
    if (m_left_tex)
        SDL_DestroyTexture(m_left_tex);
    if (m_down_tex)
        SDL_DestroyTexture(m_down_tex);
    if (m_up_tex)
        SDL_DestroyTexture(m_up_tex);
    if (m_a_tex)
        SDL_DestroyTexture(m_a_tex);
    if (m_b_tex)
        SDL_DestroyTexture(m_b_tex);
    if (m_start_tex)
        SDL_DestroyTexture(m_start_tex);
    if (m_select_tex)
        SDL_DestroyTexture(m_select_tex);
}

void InputDebugWindow::render(u8 input_bitmask)
{
    SDL_Rect SrcR;
    SDL_Rect DestR;

    SrcR.x = 0;
    SrcR.y = 0;
    SrcR.w = 15;
    SrcR.h = 15;

    DestR.x = 0;
    DestR.y = 0;
    DestR.w = 15;
    DestR.h = 15;

    // Up/down = 8x13
    // Left/right = 14x8

    if (input_bitmask & KEY_DOWN_MASK) {
        SDL_SetTextureColorMod(m_down_tex, 255, 0, 0);
    } else {
        SDL_SetTextureColorMod(m_down_tex, 0, 0, 0);
    }

    if (input_bitmask & KEY_UP_MASK) {
        SDL_SetTextureColorMod(m_up_tex, 255, 0, 0);
    } else {
        SDL_SetTextureColorMod(m_up_tex, 0, 0, 0);
    }

    if (input_bitmask & KEY_LEFT_MASK) {
        SDL_SetTextureColorMod(m_left_tex, 255, 0, 0);
    } else {
        SDL_SetTextureColorMod(m_left_tex, 0, 0, 0);
    }

    if (input_bitmask & KEY_RIGHT_MASK) {
        SDL_SetTextureColorMod(m_right_tex, 255, 0, 0);
    } else {
        SDL_SetTextureColorMod(m_right_tex, 0, 0, 0);
    }

    SDL_RenderCopy(m_renderer, m_down_tex, &SrcR, &DestR);
    DestR.x += 15;
    SDL_RenderCopy(m_renderer, m_up_tex, &SrcR, &DestR);
    DestR.x += 15;
    SDL_RenderCopy(m_renderer, m_left_tex, &SrcR, &DestR);
    DestR.x += 15;
    SDL_RenderCopy(m_renderer, m_right_tex, &SrcR, &DestR);
}
