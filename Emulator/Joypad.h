//
// Created by Spencer Dixon on 6/1/20.
//

#pragma once

#include "IODevice.h"

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#    include <SDL.h>
#    pragma clang diagnostic pop
#else
#    include <SDL.h>
#endif

#include <SD/LogStream.h>
#include <SD/Types.h>

// Overview of how Joypad I/O Works:
//
// Bit 7 - Not used
// Bit 6 - Not used
// Bit 5 - P15 Select Button Keys      (0=Select)
// Bit 4 - P14 Select Direction Keys   (0=Select)
// Bit 3 - P13 Input Down  or Start    (0=Pressed) (Read Only)
// Bit 2 - P12 Input Up    or Select   (0=Pressed) (Read Only)
// Bit 1 - P11 Input Left  or Button B (0=Pressed) (Read Only)
// Bit 0 - P10 Input Right or Button A (0=Pressed) (Read Only)

enum class Key : u8 {
    Right,
    Left,
    Up,
    Down,
    A,
    B,
    Start,
    Select,
    _Count,
};

enum class JoypadReadMode {
    DPad,
    Button,
    None,
};

// clang-format off
// When in DPad mode:
constexpr u8 KEY_RIGHT_MASK   = (1 << 0);
constexpr u8 KEY_LEFT_MASK    = (1 << 1);
constexpr u8 KEY_UP_MASK      = (1 << 2);
constexpr u8 KEY_DOWN_MASK    = (1 << 3);

// When in Button mode:
constexpr u8 KEY_A_MASK       = (1 << 0);
constexpr u8 KEY_B_MASK       = (1 << 1);
constexpr u8 KEY_SELECT_MASK  = (1 << 2);
constexpr u8 KEY_START_MASK   = (1 << 3);
// clang-format on

class InputDebugWindow;

class Joypad final : public IODevice {
public:
    friend InputDebugWindow;
    Joypad();

    // IODevice Interface
    u8 read(u16 address) override;
    void write(u16 address, u8 value) override;

    // Interface with SDL
    void set_key_state(const Key& key, bool is_down);

protected:
    void set_mode(JoypadReadMode mode) { m_mode = mode; }

    bool is_right_down() const { return m_keys[static_cast<int>(Key::Right)]; }
    bool is_left_down() const { return m_keys[static_cast<int>(Key::Left)]; }
    bool is_up_down() const { return m_keys[static_cast<int>(Key::Up)]; }
    bool is_down_down() const { return m_keys[static_cast<int>(Key::Down)]; }
    bool is_a_down() const { return m_keys[static_cast<int>(Key::A)]; }
    bool is_b_down() const { return m_keys[static_cast<int>(Key::B)]; }
    bool is_start_down() const { return m_keys[static_cast<int>(Key::Start)]; }
    bool is_select_down() const { return m_keys[static_cast<int>(Key::Select)]; }

private:
    JoypadReadMode m_mode;
    bool m_keys[static_cast<int>(Key::_Count)];
};

class InputDebugWindow {

public:
    InputDebugWindow(SDL_Renderer* renderer);
    ~InputDebugWindow();

    void render(Joypad*);

private:
    SDL_Renderer* m_renderer { nullptr };

    // Textures
    SDL_Texture* m_left_tex { nullptr };
    SDL_Texture* m_right_tex { nullptr };
    SDL_Texture* m_up_tex { nullptr };
    SDL_Texture* m_down_tex { nullptr };
    SDL_Texture* m_a_tex { nullptr };
    SDL_Texture* m_b_tex { nullptr };
    SDL_Texture* m_start_tex { nullptr };
    SDL_Texture* m_select_tex { nullptr };
};

const LogStream& operator<<(const LogStream& stream, const Joypad& input);
