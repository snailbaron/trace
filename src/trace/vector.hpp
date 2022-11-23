#pragma once

#include <cmath>
#include <ostream>
#include <sstream>
#include <string>

struct Vector {
    Vector() = default;
    Vector(float x, float y, float z)
        : x(x), y(y), z(z)
    { }

    Vector& operator+=(const Vector& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector& operator-=(const Vector& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

inline Vector operator+(Vector lhs, const Vector& rhs)
{
    lhs += rhs;
    return lhs;
}

inline Vector operator-(Vector lhs, const Vector& rhs)
{
    lhs -= rhs;
    return lhs;
}

inline Vector operator*(Vector v, float scalar)
{
    v *= scalar;
    return v;
}

inline Vector operator*(float scalar, Vector v)
{
    v *= scalar;
    return v;
}

inline Vector operator/(Vector v, float scalar)
{
    v /= scalar;
    return v;
}

inline float sqlen(const Vector& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline float len(const Vector& v)
{
    return std::sqrt(sqlen(v));
}

inline Vector unit(const Vector& v)
{
    if (auto sql = sqlen(v); sql > 0) {
        return v / std::sqrt(sql);
    }
    return v;
}

inline float sqdistance(const Vector& lhs, const Vector& rhs)
{
    return sqlen(lhs - rhs);
}

inline float distance(const Vector& lhs, const Vector& rhs)
{
    return len(lhs - rhs);
}

inline float dot(const Vector& u, const Vector& v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline Vector cross(const Vector& u, const Vector& v)
{
    return Vector{
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x,
    };
}

inline std::ostream& operator<<(std::ostream& output, const Vector& v)
{
    return output << "{" << v.x << ", " << v.y << ", " << v.z << "}";
}

inline std::string toString(const Vector& v)
{
    std::ostringstream stream;
    stream << v;
    return stream.str();
}
