//
// Created by Spencer Dixon on 6/8/20.
//

#pragma once

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#    include <SDL.h>
#    include <SDL_image.h>
#    pragma clang diagnostic pop
#else
#    include <SDL.h>
#    include <SDL_image.h>
#endif
