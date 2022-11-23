#pragma once

#include <SDL.h>

#include <stdexcept>

template <class T>
T* sdlCheck(T* ptr)
{
    if (!ptr) {
        throw std::runtime_error{SDL_GetError()};
    }
    return ptr;
}

inline void sdlCheck(int returnCode)
{
    if (returnCode != 0) {
        throw std::runtime_error{SDL_GetError()};
    }
}
