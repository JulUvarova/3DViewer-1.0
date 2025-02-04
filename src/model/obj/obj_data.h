#pragma once

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace s21 {

struct Vec3 {
  float x, y, z;
  Vec3() = default;
  Vec3(float x, float y, float z) : x(x), y(y), z(z) {};
};

struct Vec2 {
  float x, y;
  Vec2() = default;
  Vec2(float x, float y) : x(x), y(y) {};
};

struct VertexIndices {
  int v{-1};   // index of vertices, -1 if none
  int vt{-1};  // index of texcoords, -1 if none
  int vn{-1};  // index of normals, -1 if none
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
  Object* current_object_ = nullptr;
  Mesh* current_mesh_ = nullptr;
  
  std::string Trim(const std::string& str);

  void ProcessLine(std::string_view line);

  void ParseVertex(const std::vector<std::string_view>& tokens);

  void ParseNormal(const std::vector<std::string_view>& tokens);

  void ParseTexCoord(const std::vector<std::string_view>& tokens);

  Object* HandleObject(const std::vector<std::string_view>& tokens);

  Mesh* HandleUseMtl(const std::vector<std::string_view>& tokens,
                     Object* current_object);

  void HandleFace(const std::vector<std::string_view>& tokens,
                  Object*& current_object, Mesh*& current_mesh);

  int ParseIndex(const std::string_view& part, size_t current_count);

  std::vector<std::string> Split(const std::string_view& s, char delimiter);
};
}  // namespace s21