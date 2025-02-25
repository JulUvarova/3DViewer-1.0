#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <string>

#include "obj_data.h"
#include "exceptions.h"

// Helper function to create a temporary OBJ file with given content
std::string CreateTempObjFile(const std::string& content) {
  std::string filename = "temp_test.obj";
  std::ofstream out(filename);
  out << content;
  out.close();
  return filename;
}

// Sample OBJ content for testing
const char* sample_obj_content = R"(
# Sample OBJ file
v 0.0 0.0 0.0
v 2.0 0.0 0.0
v 0.0 2.0 0.0
vt 0.0 0.0
vt 2.0 0.0
vt 0.0 2.0
vn 0.0 0.0 2.0
o Triangle
f 1/1/1 2/2/1 3/3/1
)";

// Test: Verify that a valid OBJ file is parsed correctly.
TEST(OBJDataParserTest, ParseValidOBJFile) {
  std::string filename = CreateTempObjFile(sample_obj_content);
  s21::OBJData objData;
  objData.Parse(filename);

  // Clean up temporary file
  std::remove(filename.c_str());

  // Verify counts of parsed elements.
  EXPECT_EQ(objData.vertices.size(), 3);
  EXPECT_EQ(objData.texcoords.size(), 3);
  EXPECT_EQ(objData.normals.size(), 1);
  EXPECT_EQ(objData.objects.size(), 1);

  // Verify that the object name is parsed correctly.
  EXPECT_EQ(objData.objects[0].name, "Triangle");

  // Ensure that the object has at least one mesh with one face.
  ASSERT_FALSE(objData.objects[0].meshes.empty());
  const s21::Mesh& mesh = objData.objects[0].meshes[0];
  ASSERT_FALSE(mesh.faces.empty());
  const s21::Face& face = mesh.faces[0];
  EXPECT_EQ(face.vertices.size(), 3);

  // Check that the vertex indices have been converted to zero-based indices.
  EXPECT_EQ(face.vertices[0].v, 0);
  EXPECT_EQ(face.vertices[0].vt, 0);
  EXPECT_EQ(face.vertices[0].vn, 0);

  EXPECT_EQ(face.vertices[1].v, 1);
  EXPECT_EQ(face.vertices[1].vt, 1);
  EXPECT_EQ(face.vertices[1].vn, 0);

  EXPECT_EQ(face.vertices[2].v, 2);
  EXPECT_EQ(face.vertices[2].vt, 2);
  EXPECT_EQ(face.vertices[2].vn, 0);
}

// Test: Check that normalization adjusts vertex bounds appropriately.
TEST(OBJDataParserTest, NormalizeVertices) {
  std::string filename = CreateTempObjFile(sample_obj_content);
  s21::OBJData objData;
  objData.Parse(filename);
  std::remove(filename.c_str());

  // Invoke normalization on the parsed vertices.
  objData.Normalize();

  for(const auto& vertex : objData.vertices)
  {
    EXPECT_GE(vertex.x, -1.0);
    EXPECT_LE(vertex.x, 1.0);
    EXPECT_GE(vertex.y, -1.0);
    EXPECT_LE(vertex.y, 1.0);
    EXPECT_GE(vertex.z, -1.0);
    EXPECT_LE(vertex.z, 1.0);
  }
}

// Test: Ensure that toString() returns a non-empty representation of the data.
TEST(OBJDataParserTest, ToStringReturnsData) {
  std::string filename = CreateTempObjFile(sample_obj_content);
  s21::OBJData objData;
  objData.Parse(filename);
  std::remove(filename.c_str());

  std::string output = objData.toString();
  EXPECT_FALSE(output.empty());
  EXPECT_NE(output.find("Vertices count: 3"), std::string::npos);
  EXPECT_NE(output.find("Objects count: 1"), std::string::npos);
  EXPECT_NE(output.find("Object: Triangle"), std::string::npos);
  EXPECT_NE(output.find("Faces count: 1"), std::string::npos);
}

// Test: Parsing a non-existent file should result in no data being loaded.
TEST(OBJDataParserTest, ParseNonexistentFile) {
  s21::OBJData objData;
  EXPECT_THROW(objData.Parse("nonexistent.obj"), s21::MeshLoadException);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
