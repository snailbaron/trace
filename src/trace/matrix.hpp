#pragma once

#include "vector.hpp"

#include <array>
#include <cstddef>

class Matrix {
public:
    Matrix() = default;
    Matrix(
            float x00, float x01, float x02,
            float x10, float x11, float x12,
            float x20, float x21, float x22)
        : _xs{x00, x01, x02, x10, x11, x12, x20, x21, x22}
    { }

    float& operator()(size_t i, size_t j)
    {
        return _xs[i * 3 + j];
    }

    const float& operator()(size_t i, size_t j) const
    {
        return _xs[i * 3 + j];
    }

    Matrix& operator*=(const Matrix& other);

private:
    std::array<float, 9> _xs;
};

inline Matrix operator*(const Matrix& u, const Matrix& v)
{
    auto result = Matrix{};
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            result(i, j) = 0;
            for (size_t k = 0; k < 3; k++) {
                result(i, j) += u(i, k) * v(k, j);
            }
        }
    }
    return result;
}

inline Matrix& Matrix::operator*=(const Matrix& other)
{
    *this = *this * other;
    return *this;
}

inline Vector operator*(const Matrix& m, const Vector& v)
{
    return Vector{
        m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z,
        m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z,
        m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z,
    };
}
