#include "obj_data.h"
#include "../exceptions.h"

namespace s21 {

void OBJData::Normalize() {
  using namespace ranges;

  // Ensure there is at least one vertex to avoid undefined behavior.
  if (vertices.empty()) {
    return;
  }

  // Find min and max for x coordinate.
  auto [min_x_it, max_x_it] = minmax_element(
      vertices, [](const Vec3f& a, const Vec3f& b) { return a.x < b.x; });
  float min_x = min_x_it->x;
  float max_x = max_x_it->x;

  // Find min and max for y coordinate.
  auto [min_y_it, max_y_it] = minmax_element(
      vertices, [](const Vec3f& a, const Vec3f& b) { return a.y < b.y; });
  float min_y = min_y_it->y;
  float max_y = max_y_it->y;

  // Find min and max for z coordinate.
  auto [min_z_it, max_z_it] = minmax_element(
      vertices, [](const Vec3f& a, const Vec3f& b) { return a.z < b.z; });
  float min_z = min_z_it->z;
  float max_z = max_z_it->z;

  // Compute midpoints.
  float mid_x = (max_x + min_x) / 2.0f;
  float mid_y = (max_y + min_y) / 2.0f;
  float mid_z = (max_z + min_z) / 2.0f;

  // Compute the maximum distance from the midpoint along any axis.
  float dx = std::max(std::abs(max_x - mid_x), std::abs(min_x - mid_x));
  float dy = std::max(std::abs(max_y - mid_y), std::abs(min_y - mid_y));
  float dz = std::max(std::abs(max_z - mid_z), std::abs(min_z - mid_z));

  // Scale factor to normalize within a unit cube (multiplied by 2.0 for full
  // range).
  float scale_factor = std::max({dx, dy, dz}) * 2.0f;

  // Adjust and scale each vertex using ranges::for_each.
  for_each(vertices, [=](Vec3f& vertex) {
    vertex.x = (vertex.x - mid_x) / scale_factor;
    vertex.y = (vertex.y - mid_y) / scale_factor;
    vertex.z = (vertex.z - mid_z) / scale_factor;
  });
}

void OBJData::Parse(const std::string& filename) {
  // Memory mapping

  int fd = open(filename.c_str(), O_RDONLY);
  if (fd == -1) {
    throw MeshLoadException("Failed to open file: " + filename);
  }
  size_t size = lseek(fd, 0, SEEK_END);
  char* buffer =
      static_cast<char*>(mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0));
      if (buffer == MAP_FAILED) {
    close(fd);
    throw MeshLoadException("Failed to map file: " + filename);
  }
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
  using namespace ranges;

  auto parts_view =
      views::split(str, delimiter) |
      views::transform([](auto&& subrange) -> std::string_view {
        return std::string_view(&*subrange.begin(), ranges::distance(subrange));
      });

  return parts_view | to<std::vector<std::string_view>>();
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