//
// Created by Spencer Dixon on 6/1/20.
//

#include "Joypad.h"
#include <SD/Assertions.h>
#include <SD/Bytes.h>

#include <stdio.h>

Joypad::Joypad()
    : m_mode(JoypadReadMode::None)
{
    for (int i = 0; i < static_cast<int>(Key::_Count); ++i)
        m_keys[i] = false;
}

u8 Joypad::in([[maybe_unused]] u16 address)
{
    // Note: We only have 1 address (0xff00) being used for Joypad
    // related logic so we don't need to use the address
    // and it can be safely ignored.

    // Remember this weirdness:
    // 0 -> pressed   1 -> not pressed

    u8 result = 0x0f; // Assume none pressed

    if (m_mode == JoypadReadMode::DPad) {
        result |= 0x20;

        if (is_down_down())
            result ^= KEY_DOWN_MASK;
        if (is_up_down())
            result ^= KEY_UP_MASK;
        if (is_right_down())
            result ^= KEY_RIGHT_MASK;
        if (is_left_down())
            result ^= KEY_LEFT_MASK;
    } else if (m_mode == JoypadReadMode::Button) {
        result |= 0x10;

        if (is_a_down())
            result ^= KEY_A_MASK;
        if (is_b_down())
            result ^= KEY_B_MASK;
        if (is_start_down())
            result ^= KEY_START_MASK;
        if (is_select_down())
            result ^= KEY_SELECT_MASK;
    } else {
        dbg() << "Program was trying to read Joypad data but didn't have the IODevice "
              << "in a proper state to be able to get useful data back (0x10 or 0x20)";
        ASSERT_NOT_REACHED();
    }

    return result;
}

void Joypad::out([[maybe_unused]] u16 address, u8 value)
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
    stream << "Joypad Bitmask: " << to_bits(const_cast<Joypad&>(input).in(0));
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

void InputDebugWindow::render(Joypad* joypad)
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

    if (joypad->is_down_down()) {
        SDL_SetTextureColorMod(m_down_tex, 255, 0, 0);
    } else {
        SDL_SetTextureColorMod(m_down_tex, 0, 0, 0);
    }

    if (joypad->is_up_down()) {
        SDL_SetTextureColorMod(m_up_tex, 255, 0, 0);
    } else {
        SDL_SetTextureColorMod(m_up_tex, 0, 0, 0);
    }

    if (joypad->is_left_down()) {
        SDL_SetTextureColorMod(m_left_tex, 255, 0, 0);
    } else {
        SDL_SetTextureColorMod(m_left_tex, 0, 0, 0);
    }

    if (joypad->is_right_down()) {
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
