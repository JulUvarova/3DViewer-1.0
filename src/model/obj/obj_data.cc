#include "obj_data.h"

namespace s21 {

void OBJData::parse(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    throw std::runtime_error("Failed to open file: " + filename);
  }

  constexpr size_t kVertexReserve = 65536;
  vertices.reserve(kVertexReserve);

  Object* current_object = nullptr;
  Mesh* current_mesh = nullptr;
  std::unordered_map<std::string, Mesh*> material_map;

  std::string line;
  while (std::getline(file, line)) {
    std::string_view trimmed = Trim(line);
    if (trimmed.empty() || trimmed[0] == '#') {
      continue;
    }

    auto tokens = Split(trimmed, ' ');
    if (tokens.empty()) {
      continue;
    }

    const std::string_view keyword = tokens[0];

    if (keyword == "v") {
      ParseVertex(tokens);
    } else if (keyword == "vn") {
      ParseNormal(tokens);
    } else if (keyword == "vt") {
      ParseTexCoord(tokens);
    } else if (keyword == "o") {
      current_object = &objects.emplace_back();
      current_object->name = std::string(tokens[1]);
    } else if (keyword == "usemtl") {
      if (!current_object) current_object = &objects.emplace_back();
      auto& mesh = current_object->meshes.emplace_back();
      mesh.material = std::string(tokens[1]);
      material_map[mesh.material] = &mesh;
      current_mesh = &mesh;
    } else if (keyword == "f") {
      if (!current_object) current_object = &objects.emplace_back();
      if (!current_mesh) current_mesh = &current_object->meshes.emplace_back();
      HandleFace(tokens, *current_object, *current_mesh);
    }
  }
}

std::string_view OBJData::Trim(std::string_view str) {
  size_t start = str.find_first_not_of(" \t");
  size_t end = str.find_last_not_of(" \t");
  return (start == std::string_view::npos) ? ""
                                           : str.substr(start, end - start + 1);
}

void OBJData::ParseVertex(const std::vector<std::string_view>& tokens) {
  if (tokens.size() < 4) throw std::runtime_error("Invalid vertex format");
  vertices.emplace_back(std::stof(std::string(tokens[1])),
                        std::stof(std::string(tokens[2])),
                        std::stof(std::string(tokens[3])));
}

int OBJData::ParseIndex(std::string_view part, size_t current_count) {
  if (current_count == 0) {
    return -1;
  }
  int idx = 0;
  auto [ptr, ec] = std::from_chars(part.data(), part.data() + part.size(), idx);
  if (ec != std::errc()) {
    return -1;
  }
  return idx < 0 ? idx + current_count : idx - 1;
}

std::vector<std::string_view> OBJData::Split(std::string_view str,
                                             char delimiter) {
  std::vector<std::string_view> parts;
  size_t start = 0, end;
  while ((end = str.find(delimiter, start)) != std::string_view::npos) {
    parts.emplace_back(str.substr(start, end - start));
    start = end + 1;
  }
  parts.emplace_back(str.substr(start));
  return parts;
}

void OBJData::ParseNormal(const std::vector<std::string_view>& tokens) {
  if (tokens.size() < 4) {
    throw std::runtime_error("Invalid normal format");
  }
  normals.emplace_back(std::stof(std::string(tokens[1])),
                       std::stof(std::string(tokens[2])),
                       std::stof(std::string(tokens[3])));
}

void OBJData::ParseTexCoord(const std::vector<std::string_view>& tokens) {
  if (tokens.size() < 3) {
    throw std::runtime_error("Invalid texture coordinate format");
  }
  texcoords.emplace_back(std::stof(std::string(tokens[1])),
                         std::stof(std::string(tokens[2])));
}

void OBJData::HandleFace(const std::vector<std::string_view>& tokens,
                         Object& current_object, Mesh& current_mesh) {
  if (tokens.size() < 4) {
    throw std::runtime_error("Invalid face format");
  }

  Face face;
  for (size_t i = 1; i < tokens.size(); ++i) {
    auto indices = Split(tokens[i], '/');
    VertexIndices vi;

    if (!indices[0].empty()) vi.v = ParseIndex(indices[0], vertices.size());
    if (indices.size() > 1 && !indices[1].empty())
      vi.vt = ParseIndex(indices[1], texcoords.size());
    if (indices.size() > 2 && !indices[2].empty())
      vi.vn = ParseIndex(indices[2], normals.size());

    face.vertices.push_back(vi);
  }
  current_mesh.faces.push_back(face);
}

}  // namespace s21