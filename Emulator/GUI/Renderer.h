//
// Created by Spencer Dixon on 6/9/20.
//

#include <SD/String.h>
#include <SD/Types.h>

struct WindowSettings {
    u16 width;
    u16 height;
    bool resizeable;
    String title;
};

struct Rect {
    int x;
    int y;
    int width;
    int height;
};

class Renderer {
public:
//    virtual static Renderer& the();

    virtual void init() = 0;
    virtual void clear(const Color&) = 0;
    virtual void swap() = 0;

    // API Ideas:
    // void draw(Texture);
    // void fill_rect(Rect rect, Color color);
private:
};

//class SDLRenderer final : public Renderer {
//    static SDLRenderer& the();
//    SDLRenderer();
//
//    virtual void clear(const Color&) override;
//    virtual void swap() override;
//};