#pragma once

#include "vector.hpp"

#include <cassert>
#include <optional>
#include <stdexcept>
#include <iostream>

struct Sphere {
    Vector center;
    float radius = 0.f;
};

struct Reflection {
    Vector point;
    Vector normal;
    float distance = 0.f;
};

inline float pointLineDistance(
    const Vector& point, const Vector& lineOrigin, Vector lineDirection)
{
    return len(cross(lineOrigin - point, lineOrigin + lineDirection - point));
}

inline std::optional<Reflection> castRay(
    const Vector& origin, Vector direction, const Sphere& sphere)
{
    direction = unit(direction);

    const auto& r = sphere.radius;

    const auto d = pointLineDistance(sphere.center, origin, direction);
    if (d > sphere.radius) {
        return std::nullopt;
    }

    const auto centerCoord = dot(direction, sphere.center - origin);
    const auto reflectionCoord = centerCoord - std::sqrt(r * r - d * d);
    const auto reflectionPoint = origin + direction * reflectionCoord;
    const auto normal = unit(reflectionPoint - sphere.center);

    return Reflection{
        .point = reflectionPoint,
        .normal = normal,
        .distance = reflectionCoord};
}
