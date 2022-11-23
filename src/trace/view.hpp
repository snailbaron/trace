#pragma once

#include "geometry.hpp"
#include "sdl.hpp"
#include "vector.hpp"
#include "world.hpp"

#include <algorithm>
#include <iostream>

namespace config {
    const int windowWidth = 960;
    const int windowHeight = 540;
    const float aspectRatio = 1.f * windowWidth / windowHeight;
    const int pixelZoom = 4;
    const int windowHorizPoints = windowWidth / pixelZoom;
    const int windowVertPoints = windowHeight / pixelZoom;
    const float distanceToScreen = 1.f;
    const float screenWidth = 1.f;
    const float pixelWorldSize = screenWidth / windowHorizPoints;
    const float screenHeight = screenWidth / aspectRatio;
    const Vector lightDirection = unit({1, 1, -1});
    const SDL_Color materialColor {255, 180, 0, 255};
};

class View {
public:
    View()
    {
        _window = sdlCheck(SDL_CreateWindow(
            "Trace",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            config::windowWidth,
            config::windowHeight,
            0));
        _renderer = sdlCheck(SDL_CreateRenderer(
            _window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));
    }

    bool processInput()
    {
        for (auto e = SDL_Event{}; SDL_PollEvent(&e); ) {
            if (e.type == SDL_QUIT ||
                    e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q &&
                        (e.key.keysym.mod & (KMOD_CTRL | KMOD_ALT | KMOD_SHIFT)) == 0) {
                return false;
            }
        }

        return true;
    }

    void present(const World& world)
    {
        const auto& origin = world.player.position;
        const auto look = unit(world.player.direction);

        const auto screenCenter =
            origin + unit(world.player.direction) * config::distanceToScreen;

        const auto deltaX = unit(Vector{look.y, -look.x, 0}) * config::pixelWorldSize;
        const auto deltaY = unit(Vector{
            look.x * look.z,
            look.y * look.z,
            -(look.x * look.x + look.y * look.y)
        }) * config::pixelWorldSize;

        const float middleX = config::windowHorizPoints / 2.f - 0.5f;
        const float middleY = config::windowVertPoints / 2.f - 0.5f;

        std::vector<Sphere> spheres = world.spheres;
        std::sort(
            spheres.begin(),
            spheres.end(),
            [&origin] (const auto& u, const auto& v) {
                return sqdistance(origin, u.center) - u.radius <
                    sqdistance(origin, v.center) - v.radius;
            });

        sdlCheck(SDL_SetRenderDrawColor(_renderer, 50, 50, 50, 255));
        sdlCheck(SDL_RenderClear(_renderer));

        for (float x = 0; x < config::windowHorizPoints; x++) {
            for (float y = 0; y < config::windowVertPoints; y++) {
                auto screenPinPoint = screenCenter +
                    deltaX * (x - middleX) + deltaY * (y - middleY);
                //std::cerr << x << ":" << y << ": pin " << screenPinPoint << "\n";
                auto rayDirection = unit(screenPinPoint - origin);

                std::optional<Reflection> bestReflection;
                for (const auto& sphere : spheres) {
                    if (bestReflection &&
                            distance(origin, sphere.center) - sphere.radius >
                                bestReflection->distance) {
                        break;
                    }

                    auto reflection = castRay(origin, rayDirection, sphere);
                    if (reflection && (!bestReflection ||
                            reflection->distance < bestReflection->distance)) {
                        bestReflection = reflection;
                    }
                }

                if (bestReflection) {
                    auto brightness =
                        (-dot(config::lightDirection, bestReflection->normal) + 1.f) / 2;

                    SDL_FRect rect {
                        .x = x * config::pixelZoom,
                        .y = y * config::pixelZoom,
                        .w = config::pixelZoom,
                        .h = config::pixelZoom,
                    };

                    sdlCheck(SDL_SetRenderDrawColor(
                        _renderer,
                        static_cast<Uint8>((float)config::materialColor.r * brightness),
                        static_cast<Uint8>((float)config::materialColor.g * brightness),
                        static_cast<Uint8>((float)config::materialColor.b * brightness),
                        255));
                    sdlCheck(SDL_RenderFillRectF(_renderer, &rect));
                }

            }
        }

        SDL_RenderPresent(_renderer);
    }

private:
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
};

