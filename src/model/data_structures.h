#pragma once

#include <algorithm>
#include <array>
#include <cmath>

/**
 * @namespace s21
 * @brief Contains vector and matrix structures and operations for 2D, 3D, and
 * 4D computations.
 *
 * This namespace provides implementations of 2D, 3D, and 4D vectors, as well as
 * a 4x4 matrix, commonly used in computer graphics, game development, and
 * scientific computing.
 */
namespace s21 {

// 2D Vector
/**
 * @struct Vec2f
 * @brief A 2D vector with x and y components.
 *
 * This structure represents a 2D vector and supports basic operations such as
 * addition, subtraction, and scalar multiplication.
 */
struct Vec2f {
  float x{0.0f},  ///< The x-component of the vector, initialized to 0.0f.
      y{0.0f};    ///< The y-component of the vector, initialized to 0.0f.

  /**
   * @brief Default constructor, initializes the vector to (0, 0).
   */
  Vec2f() = default;

  /**
   * @brief Constructor with specific x and y values.
   * @param x The x-component.
   * @param y The y-component.
   */
  Vec2f(float x, float y) : x(x), y(y) {}

  // Default copy and move constructors and assignment operators
  explicit Vec2f(const Vec2f&) = default;
  Vec2f& operator=(const Vec2f&) = default;
  Vec2f(Vec2f&&) = default;
  Vec2f& operator=(Vec2f&&) = default;

  /**
   * @brief Destructor.
   */
  ~Vec2f() = default;

  /**
   * @brief Add another Vec2f to this vector.
   * @param other The vector to add.
   * @return A new Vec2f representing the sum of this vector and the other.
   */
  Vec2f operator+(const Vec2f& other) const {
    return {x + other.x, y + other.y};
  }

  /**
   * @brief Subtract another Vec2f from this vector.
   * @param other The vector to subtract.
   * @return A new Vec2f representing the difference between this vector and the
   * other.
   */
  Vec2f operator-(const Vec2f& other) const {
    return {x - other.x, y - other.y};
  }

  /**
   * @brief Multiply this vector by a scalar.
   * @param scalar The scalar value to multiply by.
   * @return A new Vec2f representing this vector scaled by the scalar.
   */
  Vec2f operator*(float scalar) const { return {x * scalar, y * scalar}; }
};

// 3D Vector
/**
 * @struct Vec3f
 * @brief A 3D vector with x, y, and z components.
 *
 * This structure represents a 3D vector and provides operations such as
 * addition, subtraction, scalar multiplication, length calculation, and
 * normalization.
 *
 * @note The copy constructor is defined with an empty body, which does not
 * initialize members and may lead to undefined behavior. It is recommended to
 * use `= default` or properly implement it (e.g., `Vec3f(const Vec3f& other) :
 * x(other.x), y(other.y), z(other.z) {}`).
 */
struct Vec3f {
  float x{0.0f},  ///< The x-component of the vector, initialized to 0.0f.
      y{0.0f},    ///< The y-component of the vector, initialized to 0.0f.
      z{0.0f};    ///< The z-component of the vector, initialized to 0.0f.

  /**
   * @brief Default constructor, initializes the vector to (0, 0, 0).
   */
  Vec3f() = default;

  /**
   * @brief Constructor with specific x, y, and z values.
   * @param x The x-component.
   * @param y The y-component.
   * @param z The z-component.
   */
  Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}

  /**
   * @brief Copy constructor.
   * @param other The vector to copy from.
   * @note This implementation is empty, leaving members uninitialized, which is
   * likely a bug. Consider using `= default` or proper initialization.
   */
  explicit Vec3f(const Vec3f&) {}

  // Default assignment operators
  Vec3f& operator=(const Vec3f&) = default;
  Vec3f(Vec3f&&) = default;
  Vec3f& operator=(Vec3f&&) = default;

  /**
   * @brief Add another Vec3f to this vector.
   * @param other The vector to add.
   * @return A new Vec3f representing the sum of this vector and the other.
   */
  Vec3f operator+(const Vec3f& other) const {
    return {x + other.x, y + other.y, z + other.z};
  }

  /**
   * @brief Subtract another Vec3f from this vector.
   * @param other The vector to subtract.
   * @return A new Vec3f representing the difference between this vector and the
   * other.
   */
  Vec3f operator-(const Vec3f& other) const {
    return {x - other.x, y - other.y, z - other.z};
  }

  /**
   * @brief Multiply this vector by a scalar.
   * @param scalar The scalar value to multiply by.
   * @return A new Vec3f representing this vector scaled by the scalar.
   */
  Vec3f operator*(float scalar) const {
    return {x * scalar, y * scalar, z * scalar};
  }

  /**
   * @brief Calculate the Euclidean length of the vector.
   * @return The length of the vector as a float.
   */
  float length() const { return std::sqrt(x * x + y * y + z * z); }

  /**
   * @brief Normalize the vector to unit length.
   * @return A new Vec3f with the same direction but length 1. If the vector's
   * length is zero, returns the original vector.
   */
  Vec3f normalize() const {
    float len = length();
    return len > 0.0f ? *this * (1.0f / len) : *this;
  }
};

// 4D Vector
/**
 * @struct Vec4f
 * @brief A 4D vector with x, y, z, and w components.
 *
 * This structure represents a 4D vector, commonly used for homogeneous
 * coordinates in 3D transformations. It can be constructed from a Vec3f,
 * setting w to 1.0f by default.
 */
struct Vec4f {
  float x{},  ///< The x-component of the vector, initialized to 0.0f.
      y{},    ///< The y-component of the vector, initialized to 0.0f.
      z{},    ///< The z-component of the vector, initialized to 0.0f.
      w{};    ///< The w-component of the vector, initialized to 0.0f.

  /**
   * @brief Default constructor, initializes the vector to (0, 0, 0, 0).
   */
  Vec4f() = default;

  /**
   * @brief Constructor with specific x, y, z, and w values.
   * @param x The x-component.
   * @param y The y-component.
   * @param z The z-component.
   * @param w The w-component.
   */
  Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

  /**
   * @brief Construct a Vec4f from a Vec3f, setting w to 1.0f.
   * @param other The Vec3f to copy components from.
   */
  explicit Vec4f(const Vec3f& other) {
    x = other.x;
    y = other.y;
    z = other.z;
    w = 1.0f;
  }

  /**
   * @brief Assign from a Vec3f, setting w to 1.0f.
   * @param other The Vec3f to copy components from.
   * @return A reference to this Vec4f after assignment.
   */
  Vec4f& operator=(const Vec3f& other) {
    x = other.x;
    y = other.y;
    z = other.z;
    w = 1.0f;
    return *this;
  }

  /**
   * @brief Add another Vec4f to this vector.
   * @param other The vector to add.
   * @return A new Vec4f representing the sum of this vector and the other.
   */
  Vec4f operator+(const Vec4f& other) const {
    return {x + other.x, y + other.y, z + other.z, w + other.w};
  }

  /**
   * @brief Subtract another Vec4f from this vector.
   * @param other The vector to subtract.
   * @return A new Vec4f representing the difference between this vector and the
   * other.
   */
  Vec4f operator-(const Vec4f& other) const {
    return {x - other.x, y - other.y, z - other.z, w - other.w};
  }

  /**
   * @brief Multiply this vector by a scalar.
   * @param scalar The scalar value to multiply by.
   * @return A new Vec4f representing this vector scaled by the scalar.
   */
  Vec4f operator*(float scalar) const {
    return {x * scalar, y * scalar, z * scalar, w * scalar};
  }
};

// 4x4 Matrix
/**
 * @typedef Mat4f
 * @brief A 4x4 matrix represented as a 2D array of floats.
 *
 * This type alias defines a 4x4 matrix used for linear transformations in 3D
 * space, such as rotations, translations, and scaling.
 */
using Mat4f = std::array<std::array<float, 4>, 4>;

/**
 * @brief Multiply two 4x4 matrices.
 * @param a The first matrix (left operand).
 * @param b The second matrix (right operand).
 * @return A new Mat4f representing the product of the two matrices.
 */
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

/**
 * @brief Multiply a 4x4 matrix by a 4D vector.
 * @param mat The matrix (left operand).
 * @param vec The vector (right operand).
 * @return A new Vec4f representing the transformed vector.
 */
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

/**
 * @brief Multiply a 4D vector by a 4x4 matrix.
 * @param vec The vector (left operand).
 * @param mat The matrix (right operand).
 * @return A new Vec4f representing the transformed vector, equivalent to mat *
 * vec.
 * @note This operation is defined to match mat * vec for consistency, despite
 * the reversed order.
 */
inline Vec4f operator*(const Vec4f& vec, const Mat4f& mat) { return mat * vec; }

}  // namespace s21