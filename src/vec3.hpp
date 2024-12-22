#pragma once
#include <immintrin.h>
#include <cmath>

// Ops supported
class Vec3 {
public:
    float x, y, z;

    Vec3& operator+=(const Vec3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    constexpr float l2() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }
};

constexpr static inline Vec3 operator+(Vec3 a, Vec3 b)
{
    return Vec3 {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z
    };
}

constexpr static inline Vec3 operator-(Vec3 a, Vec3 b)
{
    return Vec3 {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z
    };
}

constexpr static inline Vec3 operator*(Vec3 a, Vec3 b)
{
    return Vec3 {
        .x = a.x * b.x,
        .y = a.y * b.y,
        .z = a.z * b.z
    };
}

constexpr static inline Vec3 operator/(Vec3 a, Vec3 b)
{
    return Vec3 {
        .x = a.x / b.x,
        .y = a.y / b.y,
        .z = a.z / b.z
    };
}


constexpr static inline Vec3 operator*(Vec3 a, float b)
{
    return Vec3 {
        .x = a.x * b,
        .y = a.y * b,
        .z = a.z * b
    };
}

constexpr static inline Vec3 operator*(float a, Vec3 b)
{
    return b * a;
}

constexpr static inline Vec3 operator/(Vec3 a, float b)
{
    return Vec3 {
        .x = a.x / b,
        .y = a.y / b,
        .z = a.z / b
    };
}

constexpr static inline float dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

constexpr static inline Vec3 unit_vector(Vec3 a)
{
    return a / a.l2();
}


class Ray {
public:
    Vec3 origin, direction;
};

class Vec3_256 {
public:
    __m256 x, y, z;
};

static inline Vec3_256 operator+(Vec3_256 a, Vec3_256 b)
{
    return Vec3_256 {
        .x = _mm256_add_ps(a.x, b.x),
        .y = _mm256_add_ps(a.y, b.y),
        .z = _mm256_add_ps(a.z, b.z)
    };
}

static inline Vec3_256 operator-(Vec3_256 a, Vec3_256 b)
{
    return Vec3_256 {
        .x = _mm256_sub_ps(a.x, b.x),
        .y = _mm256_sub_ps(a.y, b.y),
        .z = _mm256_sub_ps(a.z, b.z)
    };
}

// Element-wise multiplication of two vectors
static inline Vec3_256 operator*(Vec3_256 a, Vec3_256 b)
{
    return Vec3_256 {
        .x = _mm256_mul_ps(a.x, b.x),
        .y = _mm256_mul_ps(a.y, b.y),
        .z = _mm256_mul_ps(a.z, b.z)
    };
}

static inline Vec3_256 operator*(Vec3_256 a, __m256 b)
{
    return Vec3_256 {
        .x = _mm256_mul_ps(a.x, b),
        .y = _mm256_mul_ps(a.y, b),
        .z = _mm256_mul_ps(a.z, b)
    };
}

static inline Vec3_256 operator/(Vec3_256 a, Vec3_256 b)
{
    return Vec3_256 {
        .x = _mm256_div_ps(a.x, b.x),
        .y = _mm256_div_ps(a.y, b.y),
        .z = _mm256_div_ps(a.z, b.z)
    };
}

using Point3 = Vec3;
using Color = Vec3;
