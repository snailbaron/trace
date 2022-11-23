#pragma once

#include "geometry.hpp"
#include "vector.hpp"

#include <cmath>
#include <vector>

struct Spectator {
    Vector position;
    Vector direction;
};

class World {
public:
    void createTestLevel()
    {
        player = {
            .position = {0, -10, 0},
            .direction = {0, 1, 0},
        };

        spheres.push_back({
            .center = {0, 0, 0},
            .radius = 1
        });
    }

    void update(double delta)
    {
        _time = std::fmod(_time + delta, 2.f);

        double spherePosition = 0;
        if (_time <= 1) {
            spherePosition = 4 * (_time - _time * _time);
        } else {
            spherePosition = 4 * (2 - 3 * _time + _time * _time);
        }

        for (auto& sphere : spheres) {
            sphere.center.z = (float)spherePosition;
        }
    }

    Spectator player;
    std::vector<Sphere> spheres;

private:
    double _time = 0;
};

