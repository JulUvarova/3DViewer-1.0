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

#include "../data_structures.h"
#include "../exceptions.h"
#include "../math/transform_matrix_builder.h"
#include "Logger.h"
#include "range/v3/all.hpp"

namespace s21 {

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
  std::vector<Vec3f> vertices;
  std::vector<Vec2f> texcoords;
  std::vector<Vec3f> normals;
  std::vector<Object> objects;
  float x_min = 0.0f, x_max = 0.0f, y_min = 0.0f, y_max = 0.0f, z_min = 0.0f,
        z_max = 0.0f;

  void Parse(const std::string& filename);
  void Normalize();
  std::string toString();

 private:
  Object* current_object_ = nullptr;
  Mesh* current_mesh_ = nullptr;
  std::vector<std::string> errors_;

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

  inline float ParseFloat(std::string_view sv);

  inline std::string_view TrimView(std::string_view sv);

  inline std::vector<std::string_view> SplitView(std::string_view str,
                                                 char delimiter);

  inline std::vector<std::string_view> Tokenize(std::string_view line);
};
}  // namespace s21