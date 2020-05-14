//
// Created by Spencer Dixon on 5/14/20.
//

#pragma once

struct OffscreenFrameBuffer {
    void* memory;
    int height;
    int width;
    int pitch;
};