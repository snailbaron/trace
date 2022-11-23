#include "sdl.hpp"
#include "view.hpp"
#include "world.hpp"

#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

int main()
{
    try {
        using Clock = std::chrono::high_resolution_clock;

        sdlCheck(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS));

        auto world = World{};
        world.createTestLevel();

        auto view = View{};

        const auto startTime = Clock::now();
        const auto frameDurationSec = 1.0 / 60;
        const auto frameDuration =
            std::chrono::duration_cast<Clock::duration>(
                std::chrono::duration<double>(frameDurationSec));
        int lastFrame = 0;

        for (;;) {
            if (!view.processInput()) {
                break;
            }

            int currentFrame =
                static_cast<int>((Clock::now() - startTime) / frameDuration);
            int framesPassed = currentFrame - lastFrame;
            lastFrame = currentFrame;

            if (framesPassed > 0) {
                for (int i = 0; i < framesPassed; i++) {
                    world.update(frameDurationSec);
                }
                view.present(world);
            }

            auto nextFrameTime = startTime + frameDuration * (currentFrame + 1);
            std::this_thread::sleep_until(nextFrameTime);
        }

        SDL_Quit();
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
