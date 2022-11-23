#pragma once

#include "geometry.hpp"
#include "matrix.hpp"
#include "vector.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

struct Spectator {
    float maxSpeed = 5.f;
    float accelerationTime = 1.f;
    float decelerationTime = 0.5f;

    Vector position;
    Vector direction;
    Vector velocity;
    Vector control;
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
        spheres.push_back({
            .center = {8, 0, 28},
            .radius = 1.2
        });

        columns.push_back({
            .x = 5.f,
            .y = 3.f,
            .r = 0.5f
        });
    }

    void update(float delta)
    {
        _time = std::fmod(_time + delta, 2.f);

        double spherePosition = 0;
        if (_time <= 1) {
            spherePosition = 1 * (_time - _time * _time);
        } else {
            spherePosition = 1 * (2 - 3 * _time + _time * _time);
        }

        for (auto& sphere : spheres) {
            sphere.center.z = (float)spherePosition;
        }

        const auto acceleration = player.maxSpeed / player.accelerationTime;
        const auto deceleration = player.maxSpeed / player.decelerationTime;
        player.velocity += player.control * (acceleration + deceleration) * delta;

        auto speed = len(player.velocity);
        if (speed > 0) {
            auto targetSpeed = std::clamp<float>(
                speed - deceleration * delta, 0, player.maxSpeed);
            player.velocity *= targetSpeed / speed;
            speed = targetSpeed;
        }

        if (speed > 0) {
            player.position += player.velocity * delta;
        }
    }

    void turn(float dx, float dy)
    {
        _turnAngle = std::fmod(_turnAngle + dx, float(2 * M_PI));
        if (_turnAngle < 0) {
            _turnAngle += 2 * M_PI;
        }
        _lookAngle = std::clamp(_lookAngle + dy, -(float)M_PI, (float)M_PI);

        const auto cx = std::cos(_turnAngle);
        const auto sx = std::sin(_turnAngle);
        const auto cy = std::cos(_lookAngle);
        const auto sy = std::sin(_lookAngle);

        player.direction = Vector{sx * cy, cx * cy, sy};
    }

    void move(float right, float forward)
    {
        auto playerRight = Vector{player.direction.y, -player.direction.x, 0};
        player.control =
            unit(player.direction * forward + playerRight * right);
    }

    Spectator player;
    std::vector<Sphere> spheres;
    std::vector<Column> columns;
    float _turnAngle = 0.f;
    float _lookAngle = 0.f;

private:
    double _time = 0;
};
