#pragma once

#include <algorithm>
#include <array>
#include <cmath>

namespace s21 {

// 2D Vector
struct Vec2f {
  float x{0.0f}, y{0.0f};

  Vec2f() = default;
  Vec2f(float x, float y) : x(x), y(y){};
  Vec2f(const Vec2f&) = default;
  Vec2f& operator=(const Vec2f&) = default;
  Vec2f(Vec2f&&) = default;
  Vec2f& operator=(Vec2f&&) = default;

  ~Vec2f() = default;

  Vec2f operator+(const Vec2f& other) const {
    return {x + other.x, y + other.y};
  }
  Vec2f operator-(const Vec2f& other) const {
    return {x - other.x, y - other.y};
  }
  Vec2f operator*(float scalar) const { return {x * scalar, y * scalar}; }
};

// 3D Vector
struct Vec3f {
  float x{0.0f}, y{0.0f}, z{0.0f};

  Vec3f() = default;
  Vec3f(float x, float y, float z) : x(x), y(y), z(z){};
  Vec3f(const Vec3f&) {}
  Vec3f& operator=(const Vec3f&) = default;
  Vec3f(Vec3f&&) = default;
  Vec3f& operator=(Vec3f&&) = default;

  Vec3f operator+(const Vec3f& other) const {
    return {x + other.x, y + other.y, z + other.z};
  }
  Vec3f operator-(const Vec3f& other) const {
    return {x - other.x, y - other.y, z - other.z};
  }
  Vec3f operator*(float scalar) const {
    return {x * scalar, y * scalar, z * scalar};
  }

  float length() const { return std::sqrt(x * x + y * y + z * z); }

  Vec3f normalize() const {
    float len = length();
    return len > 0.0f ? *this * (1.0f / len) : *this;
  }
};

// 4D Vector
struct Vec4f {
  float x{}, y{}, z{}, w{};

  Vec4f() = default;
  Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){};
  Vec4f(const Vec3f& other) {
    x = other.x;
    y = other.y;
    z = other.z;
    w = 1.0f;
  }

  Vec4f& operator=(const Vec3f& other) {
    x = other.x;
    y = other.y;
    z = other.z;
    w = 1.0f;
    return *this;
  }

  Vec4f operator+(const Vec4f& other) const {
    return {x + other.x, y + other.y, z + other.z, w + other.w};
  }
  Vec4f operator-(const Vec4f& other) const {
    return {x - other.x, y - other.y, z - other.z, w - other.w};
  }
  Vec4f operator*(float scalar) const {
    return {x * scalar, y * scalar, z * scalar, w * scalar};
  }
};

// 4x4 Matrix
using Mat4f = std::array<std::array<float, 4>, 4>;

// Matrix-Matrix Multiplication (External)
inline Mat4f operator*(const Mat4f& a, const Mat4f& b) {
  Mat4f result{};
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      result[row][col] = a[row][0] * b[0][col] + a[row][1] * b[1][col] +
                         a[row][2] * b[2][col] + a[row][3] * b[3][col];
    }
  }
  return result;
}
// Vector-Matrix Multiplication (External)
inline Vec4f operator*(const Mat4f& mat, const Vec4f& vec) {
  return {mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z +
              mat[0][3] * vec.w,
          mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z +
              mat[1][3] * vec.w,
          mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z +
              mat[2][3] * vec.w,
          mat[3][0] * vec.x + mat[3][1] * vec.y + mat[3][2] * vec.z +
              mat[3][3] * vec.w};
}

inline Vec4f operator*(const Vec4f& vec, const Mat4f& mat) { return mat * vec; }

}  // namespace s21
