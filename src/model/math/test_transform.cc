#include <gtest/gtest.h>

#include <cmath>

#include "data_structures.h"
#include "transform_matrix_builder.h"

using namespace s21;

//========================
// Vector Operations Tests
//========================

TEST(VectorOperationsTest, Vec2fAdditionSubtractionAndScalarMultiplication) {
  Vec2f a(1.0f, 2.0f);
  Vec2f b(3.0f, 4.0f);

  Vec2f sum = a + b;
  EXPECT_FLOAT_EQ(sum.x, 4.0f);
  EXPECT_FLOAT_EQ(sum.y, 6.0f);

  Vec2f diff = b - a;
  EXPECT_FLOAT_EQ(diff.x, 2.0f);
  EXPECT_FLOAT_EQ(diff.y, 2.0f);

  Vec2f scaled = a * 2.0f;
  EXPECT_FLOAT_EQ(scaled.x, 2.0f);
  EXPECT_FLOAT_EQ(scaled.y, 4.0f);
}

TEST(VectorOperationsTest, Vec3fOperationsAndNormalization) {
  Vec3f a(1.0f, 2.0f, 3.0f);
  Vec3f b(4.0f, 5.0f, 6.0f);

  Vec3f sum = a + b;
  EXPECT_FLOAT_EQ(sum.x, 5.0f);
  EXPECT_FLOAT_EQ(sum.y, 7.0f);
  EXPECT_FLOAT_EQ(sum.z, 9.0f);

  Vec3f diff = b - a;
  EXPECT_FLOAT_EQ(diff.x, 3.0f);
  EXPECT_FLOAT_EQ(diff.y, 3.0f);
  EXPECT_FLOAT_EQ(diff.z, 3.0f);

  Vec3f scaled = a * 3.0f;
  EXPECT_FLOAT_EQ(scaled.x, 3.0f);
  EXPECT_FLOAT_EQ(scaled.y, 6.0f);
  EXPECT_FLOAT_EQ(scaled.z, 9.0f);

  // Test length and normalization: vector (3,4,0) should have length 5 and
  // normalize to (0.6, 0.8, 0)
  Vec3f c(3.0f, 4.0f, 0.0f);
  EXPECT_FLOAT_EQ(c.length(), 5.0f);

  Vec3f normalized = c.normalize();
  EXPECT_NEAR(normalized.x, 0.6f, 1e-5);
  EXPECT_NEAR(normalized.y, 0.8f, 1e-5);
  EXPECT_NEAR(normalized.z, 0.0f, 1e-5);
}

TEST(VectorOperationsTest, Vec4fOperations) {
  // Test conversion from Vec3f: w should be set to 1
  Vec3f v3(1.0f, 2.0f, 3.0f);
  Vec4f a(v3);
  EXPECT_FLOAT_EQ(a.x, 1.0f);
  EXPECT_FLOAT_EQ(a.y, 2.0f);
  EXPECT_FLOAT_EQ(a.z, 3.0f);
  EXPECT_FLOAT_EQ(a.w, 1.0f);

  Vec4f b(4.0f, 5.0f, 6.0f, 1.0f);
  Vec4f sum = a + b;
  EXPECT_FLOAT_EQ(sum.x, 5.0f);
  EXPECT_FLOAT_EQ(sum.y, 7.0f);
  EXPECT_FLOAT_EQ(sum.z, 9.0f);
  EXPECT_FLOAT_EQ(sum.w, 2.0f);

  Vec4f diff = b - a;
  EXPECT_FLOAT_EQ(diff.x, 3.0f);
  EXPECT_FLOAT_EQ(diff.y, 3.0f);
  EXPECT_FLOAT_EQ(diff.z, 3.0f);
  EXPECT_FLOAT_EQ(diff.w, 0.0f);

  Vec4f scaled = a * 2.0f;
  EXPECT_FLOAT_EQ(scaled.x, 2.0f);
  EXPECT_FLOAT_EQ(scaled.y, 4.0f);
  EXPECT_FLOAT_EQ(scaled.z, 6.0f);
  EXPECT_FLOAT_EQ(scaled.w, 2.0f);
}

//========================
// Matrix Operations Tests
//========================

TEST(MatrixOperationsTest, Mat4fMultiplicationIdentity) {
  Mat4f identity = {{{1.0f, 0.0f, 0.0f, 0.0f},
                     {0.0f, 1.0f, 0.0f, 0.0f},
                     {0.0f, 0.0f, 1.0f, 0.0f},
                     {0.0f, 0.0f, 0.0f, 1.0f}}};
  Mat4f A = {{{2.0f, 3.0f, 4.0f, 5.0f},
              {1.0f, 0.0f, 0.0f, 0.0f},
              {0.0f, 1.0f, 0.0f, 0.0f},
              {0.0f, 0.0f, 1.0f, 0.0f}}};
  Mat4f result = identity * A;
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) EXPECT_NEAR(result[i][j], A[i][j], 1e-5);
}

TEST(MatrixOperationsTest, VectorMatrixMultiplication) {
  Mat4f identity = {{{1.0f, 0.0f, 0.0f, 0.0f},
                     {0.0f, 1.0f, 0.0f, 0.0f},
                     {0.0f, 0.0f, 1.0f, 0.0f},
                     {0.0f, 0.0f, 0.0f, 1.0f}}};
  Vec4f v{1.0f, 2.0f, 3.0f, 1.0f};
  Vec4f res = identity * v;
  EXPECT_NEAR(res.x, 1.0f, 1e-5);
  EXPECT_NEAR(res.y, 2.0f, 1e-5);
  EXPECT_NEAR(res.z, 3.0f, 1e-5);
  EXPECT_NEAR(res.w, 1.0f, 1e-5);
}

//===============================
// TransformMatrixBuilder Tests
//===============================

TEST(TransformMatrixBuilderTest, RotationMatrixZeroAngles) {
  float zero = 0.0f;
  Mat4f rot = TransformMatrixBuilder::CreateRotationMatrix(zero, zero, zero);
  // Expected identity matrix when all rotation angles are zero
  Mat4f identity = {{{1.0f, 0.0f, 0.0f, 0.0f},
                     {0.0f, 1.0f, 0.0f, 0.0f},
                     {0.0f, 0.0f, 1.0f, 0.0f},
                     {0.0f, 0.0f, 0.0f, 1.0f}}};
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) EXPECT_NEAR(rot[i][j], identity[i][j], 1e-5);
}

TEST(TransformMatrixBuilderTest, RotationMatrixZ90) {
  float zero = 0.0f;
  float pi = std::acos(-1.0f);
  Mat4f rot =
      TransformMatrixBuilder::CreateRotationMatrix(zero, zero, pi / 2.0f);
  // Expected rotation about Z-axis for 90 degrees:
  // [cos(pi/2), -sin(pi/2), 0, 0]
  // [sin(pi/2),  cos(pi/2), 0, 0]
  // [0,          0,         1, 0]
  // [0,          0,         0, 1]
  float cosVal = std::cos(pi / 2.0f);
  float sinVal = std::sin(pi / 2.0f);
  Mat4f expected = {{{cosVal, -sinVal, 0.0f, 0.0f},
                     {sinVal, cosVal, 0.0f, 0.0f},
                     {0.0f, 0.0f, 1.0f, 0.0f},
                     {0.0f, 0.0f, 0.0f, 1.0f}}};
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) EXPECT_NEAR(rot[i][j], expected[i][j], 1e-5);
}

TEST(TransformMatrixBuilderTest, MoveMatrix) {
  float tx = 3.0f, ty = -2.0f, tz = 5.0f;
  Mat4f move = TransformMatrixBuilder::CreateMoveMatrix(tx, ty, tz);
  // Verify translation values in the last column
  EXPECT_NEAR(move[0][3], tx, 1e-5);
  EXPECT_NEAR(move[1][3], ty, 1e-5);
  EXPECT_NEAR(move[2][3], tz, 1e-5);
  EXPECT_NEAR(move[3][3], 1.0f, 1e-5);

  // Verify that the upper-left 3x3 part is the identity matrix
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      EXPECT_NEAR(move[i][j], (i == j ? 1.0f : 0.0f), 1e-5);
}

TEST(TransformMatrixBuilderTest, ScaleMatrix) {
  float sx = 2.0f, sy = 3.0f, sz = 4.0f;
  Mat4f scale = TransformMatrixBuilder::CreateScaleMatrix(sx, sy, sz);
  // Verify the scaling factors on the diagonal
  EXPECT_NEAR(scale[0][0], sx, 1e-5);
  EXPECT_NEAR(scale[1][1], sy, 1e-5);
  EXPECT_NEAR(scale[2][2], sz, 1e-5);
  EXPECT_NEAR(scale[3][3], 1.0f, 1e-5);

  // Verify that off-diagonal elements are zero
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      if (i != j) EXPECT_NEAR(scale[i][j], 0.0f, 1e-5);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
