#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace s21 {

struct Vec3 {
  float x, y, z;
};

struct Vec2 {
  float x, y;
};

struct VertexIndices {
  int v;   // index of vertices, -1 if none
  int vt;  // index of texcoords, -1 if none
  int vn;  // index of normals, -1 if none
};

struct Face {
  std::vector<VertexIndices> vertices;
};

struct Mesh {
  std::string material;
  std::vector<Face> faces;
};

struct Object {
  std::string name;
  std::vector<Mesh> meshes;
};

class OBJData {
 public:
  std::vector<Vec3> vertices;
  std::vector<Vec2> texcoords;
  std::vector<Vec3> normals;
  std::vector<Object> objects;

  void parse(const std::string& filename);

 private:
  [[nodiscard]] std::string Trim(const std::string& str);

  void ParseVertex(const std::vector<std::string>& tokens);

  void ParseNormal(const std::vector<std::string>& tokens);

  void ParseTexCoord(const std::vector<std::string>& tokens);

  Object* HandleObject(const std::vector<std::string>& tokens);

  Mesh* HandleUseMtl(const std::vector<std::string>& tokens,
                     Object* current_object);

  void HandleFace(const std::vector<std::string>& tokens,
                  Object*& current_object, Mesh*& current_mesh);

  [[nodiscard]] int ParseIndex(const std::string& part, size_t current_count);

  [[nodiscard]] std::vector<std::string> Split(const std::string& s,
                                               char delimiter);
};
}  // namespace s21