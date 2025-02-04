#pragma once

#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace s21 {

struct Vec3 {
  float x{}, y{}, z{};

  Vec3() = default;  // Конструктор по умолчанию
  Vec3(float x, float y, float z)
      : x(x), y(y), z(z) {}  // Конструктор с параметрами
};

struct Vec2 {
  float x{}, y{};

  Vec2() = default;
  Vec2(float x, float y) : x(x), y(y) {}
};

struct VertexIndices {
  int v = -1;
  int vt = -1;
  int vn = -1;
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
  [[nodiscard]] std::string_view Trim(std::string_view str);
  void ParseVertex(const std::vector<std::string_view>& tokens);
  void ParseNormal(const std::vector<std::string_view>& tokens);
  void ParseTexCoord(const std::vector<std::string_view>& tokens);
  std::optional<Object> HandleObject(
      const std::vector<std::string_view>& tokens);
  std::optional<Mesh> HandleUseMtl(const std::vector<std::string_view>& tokens,
                                   Object& current_object);

  void HandleFace(const std::vector<std::string_view>& tokens,
                  Object& current_object, Mesh& current_mesh);
  [[nodiscard]] int ParseIndex(std::string_view part, size_t current_count);
  [[nodiscard]] std::vector<std::string_view> Split(std::string_view s,
                                                    char delimiter);
};
}  // namespace s21
