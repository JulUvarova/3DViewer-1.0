#include "obj_data.h"

namespace s21 {

void OBJData::Normalize() {
  float max_x = 0, max_y = 0, max_z = 0;
  float min_x = 0, min_y = 0, min_z = 0;
  for (const auto& vertex : vertices) {
    max_x = std::max(max_x, vertex.x);
    min_x = std::min(min_x, vertex.x);
    max_y = std::max(max_y, vertex.y);
    min_y = std::min(min_y, vertex.y);
    max_z = std::max(max_z, vertex.z);
    min_z = std::min(min_z, vertex.z);
  }
  float mid_x = (max_x + min_x) / 2.0f;
  float mid_y = (max_y + min_y) / 2.0f;
  float mid_z = (max_z + min_z) / 2.0f;

  max_x -= mid_x;
  max_y -= mid_y;
  max_z -= mid_z;
  min_x -= mid_x;
  min_y -= mid_y;
  min_z -= mid_z;

  // Scale the model to fit within a unit cube centered at the origin

  float scale_factor = std::max({
                           std::abs(max_x),
                           std::abs(min_x),
                           std::abs(max_y),
                           std::abs(min_y),
                           std::abs(max_z),
                           std::abs(min_z),
                       }) *
                       2.0f;

  for (auto& vertex : vertices) {
    vertex.x -= mid_x;
    vertex.y -= mid_y;
    vertex.z -= mid_z;
    vertex.x /= scale_factor;
    vertex.y /= scale_factor;
    vertex.z /= scale_factor;
  }
}

void OBJData::Parse(const std::string& filename) {
  // Memory mapping
  int fd = open(filename.c_str(), O_RDONLY);
  size_t size = lseek(fd, 0, SEEK_END);
  char* buffer =
      static_cast<char*>(mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0));
  close(fd);

  // Batch reserves
  vertices.reserve(size / 100);
  normals.reserve(size / 200);
  texcoords.reserve(size / 200);
  objects.reserve(10);

  // Process buffer
  const char* current = buffer;
  const char* end = buffer + size;
  while (current < end) {
    const char* line_start = current;
    while (current < end && *current != '\n' && *current != '\r') ++current;
    std::string_view line(line_start, current - line_start);

    while (current < end && (*current == '\r' || *current == '\n')) ++current;

    ProcessLine(line);
  }

  munmap(buffer, size);
}

void OBJData::ProcessLine(std::string_view line) {
  line = TrimView(line);
  if (line.empty() || line[0] == '#') return;

  auto tokens = Tokenize(line);
  if (tokens.empty()) return;

  const auto& keyword = tokens[0];
  if (keyword == "v") {
    ParseVertex(tokens);
  } else if (keyword == "vn") {
    ParseNormal(tokens);
  } else if (keyword == "vt") {
    ParseTexCoord(tokens);
  } else if (keyword == "o") {
    current_object_ = HandleObject(tokens);
  } else if (keyword == "usemtl") {
    current_mesh_ = HandleUseMtl(tokens, current_object_);
  } else if (keyword == "f") {
    HandleFace(tokens, current_object_, current_mesh_);
  }
}

inline void OBJData::ParseVertex(const std::vector<std::string_view>& tokens) {
  if (tokens.size() < 4) {
    return;
  }
  // Assuming the vertex coordinates are in the format "v x y z".
  vertices.emplace_back(ParseFloat(tokens[1]), ParseFloat(tokens[2]),
                        ParseFloat(tokens[3]));
}

inline void OBJData::ParseNormal(const std::vector<std::string_view>& tokens) {
  if (tokens.size() < 4) {
    return;
  }
  // Assuming the vertex coordinates are in the format "vn x y z".
  normals.emplace_back(ParseFloat(tokens[1]), ParseFloat(tokens[2]),
                       ParseFloat(tokens[3]));
}

void OBJData::ParseTexCoord(const std::vector<std::string_view>& tokens) {
  if (tokens.size() < 3) {
    return;
  }
  // Assuming the texture coordinates are in the format "vt u v".
  texcoords.emplace_back(ParseFloat(tokens[1]), ParseFloat(tokens[2]));
}

Object* OBJData::HandleObject(const std::vector<std::string_view>& tokens) {
  if (tokens.size() < 2) {
    return nullptr;
  }
  objects.emplace_back();
  Object& obj = objects.back();
  obj.name = tokens[1];
  return &obj;
}

Mesh* OBJData::HandleUseMtl(const std::vector<std::string_view>& tokens,
                            Object* current_object) {
  if (tokens.size() < 2 || !current_object) {
    return nullptr;
  }
  std::string_view material = tokens[1];
  if (current_object->meshes.empty() ||
      current_object->meshes.back().material != material) {
    current_object->meshes.emplace_back();
    current_object->meshes.back().material = material;
  }
  return &current_object->meshes.back();
}

void OBJData::HandleFace(const std::vector<std::string_view>& tokens,
                         Object*& current_object, Mesh*& current_mesh) {
  if (!current_object) {
    objects.emplace_back();
    current_object = &objects.back();
  }
  if (current_object->meshes.empty()) {
    current_object->meshes.emplace_back();
    current_mesh = &current_object->meshes.back();
  }
  if (!current_mesh) {
    return;
  }

  Face face;

  for (size_t i = 1; i < tokens.size(); ++i) {
    std::string_view part = tokens[i];
    VertexIndices vi;

    size_t delim1 = part.find('/');
    size_t delim2 = part.find('/', delim1 + 1);

    // Parse vertex index (v)
    if (delim1 != 0) {  // Check for leading '/' (e.g., "//vn")
      vi.v = ParseIndex(part.substr(0, delim1), vertices.size());
    }

    // Parse texture coordinate (vt)
    if (delim1 != std::string_view::npos && delim2 > delim1 + 1) {
      vi.vt = ParseIndex(part.substr(delim1 + 1, delim2 - delim1 - 1),
                         texcoords.size());
    }

    // Parse normal (vn)
    if (delim2 != std::string_view::npos) {
      vi.vn = ParseIndex(part.substr(delim2 + 1), normals.size());
    }

    face.vertices.push_back(vi);
  }
  current_mesh->faces.push_back(face);
}

int OBJData::ParseIndex(const std::string_view& part, size_t current_count) {
  // Check for an empty view.
  if (part.empty()) {
    return -1;
  }

  int idx = 0;
  auto [ptr, ec] = std::from_chars(part.data(), part.data() + part.size(), idx);
  if (ec != std::errc()) {
    return -1;
  }

  if (idx < 0) {
    idx += static_cast<int>(current_count);
  } else {
    idx -= 1;
  }
  if (idx < 0 || idx >= static_cast<int>(current_count)) {
    return -1;
  }
  return idx;
}

inline float OBJData::ParseFloat(std::string_view sv) {
  float value;
  auto result = std::from_chars(sv.data(), sv.data() + sv.size(), value);
  if (result.ec != std::errc()) throw std::invalid_argument("Bad float");
  return value;
}

// Returns a string view, avoiding allocation.
inline std::string_view OBJData::TrimView(std::string_view sv) {
  const char* whitespace = " \t";
  size_t start = sv.find_first_not_of(whitespace);
  if (start == std::string_view::npos) {
    return {};  // All whitespace; return empty view.
  }
  size_t end = sv.find_last_not_of(whitespace);
  return sv.substr(start, end - start + 1);
}

inline std::vector<std::string_view> OBJData::SplitView(std::string_view str,
                                                        char delimiter) {
  std::vector<std::string_view> parts;

  parts.reserve(3);
  size_t pos = 0;
  while (pos < str.size()) {
    size_t next = str.find(delimiter, pos);
    if (next == std::string_view::npos) next = str.size();
    parts.push_back(str.substr(pos, next - pos));
    pos = next + 1;
  }
  return parts;
}

inline std::vector<std::string_view> OBJData::Tokenize(std::string_view line) {
  std::vector<std::string_view> tokens;
  tokens.reserve(6);
  size_t start = 0, end = 0;

  while ((end = line.find_first_of(" \t", start)) != std::string_view::npos) {
    if (end != start) {  // Skip empty tokens
      tokens.emplace_back(line.substr(start, end - start));
    }
    start = line.find_first_not_of(" \t", end);
  }

  if (start != line.size()) {
    tokens.emplace_back(line.substr(start));
  }

  return tokens;
}

std::string OBJData::toString() {
  std::stringstream ss;
  ss << "Vertices count: " << vertices.size() << "\n"
     << "Objects count: " << objects.size() << "\n";

  for (const auto& object : objects) {
    ss << "Object: " << object.name << "\n";
    for (const auto& mesh : object.meshes) {
      ss << "  Group material: " << mesh.material << "\n"
         << "  Faces count: " << mesh.faces.size() << "\n";
    }
  }

  return ss.str();
}

}  // namespace s21