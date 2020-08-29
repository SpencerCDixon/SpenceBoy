//
// Created by spence on 8/29/20.
//

#pragma once

#pragma once

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#    include <SDL.h>
#    pragma clang diagnostic pop
#else
#    include <SDL.h>
#    include <SDL_image.h>
#    include <SDL_ttf.h>
#endif