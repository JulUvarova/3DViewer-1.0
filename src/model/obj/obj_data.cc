#include "obj_data.h"

namespace s21 {

// Reusable helper function to parse a float from a string.
// Tries std::from_chars first, falling back to std::stof if needed.
// inline float ParseFloat(const std::string_view& s) {
//   float value = 0.0f;
//   auto result = std::from_chars(s.data(), s.data() + s.size(), value);
//   if (result.ec != std::errc()) {
//     // Fallback conversion using std::stof if from_chars fails.
//     value = std::stof(std::string{s});
//   }
//   return value;
// }

inline float ParseFloat(std::string_view sv) {
  float value;
  auto result = std::from_chars(sv.data(), sv.data() + sv.size(), value);
  if (result.ec != std::errc()) throw std::invalid_argument("Bad float");
  return value;
}

// Returns a string view, avoiding allocation.
inline std::string_view TrimView(std::string_view sv) {
  const char* whitespace = " \t";
  size_t start = sv.find_first_not_of(whitespace);
  if (start == std::string_view::npos) {
    return {};  // All whitespace; return empty view.
  }
  size_t end = sv.find_last_not_of(whitespace);
  return sv.substr(start, end - start + 1);
}

inline std::vector<std::string_view> SplitView(std::string_view str,
                                               char delimiter) {
  std::vector<std::string_view> parts;
  // Reserve a small number based on typical face entries.
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

// Tokenize a line by whitespace using std::string_view.
// inline std::vector<std::string_view> Tokenize(std::string_view line) {
//   std::vector<std::string_view> tokens;
//   // A rough reservation for common line lengths (adjust as needed).
//   tokens.reserve(8);
//   size_t pos = 0;
//   while (pos < line.size()) {
//     // Skip any leading whitespace.
//     while (pos < line.size() && (line[pos] == ' ' || line[pos] == '\t')) {
//       ++pos;
//     }
//     size_t start = pos;
//     // Find the end of the token.
//     while (pos < line.size() && line[pos] != ' ' && line[pos] != '\t') {
//       ++pos;
//     }
//     if (start < pos) {
//       tokens.push_back(line.substr(start, pos - start));
//     }
//   }
//   return tokens;
// }

inline std::vector<std::string_view> Tokenize(std::string_view line) {
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

// void OBJData::parse(const std::string& filename) {
//   std::ifstream file(filename);
//   if (!file.is_open()) {
//     std::cerr << "Failed to open file: " << filename << std::endl;
//     return;
//   }

//   Object* current_object = nullptr;
//   Mesh* current_mesh = nullptr;

//   vertices.reserve(900000);
//   normals.reserve(900000);
//   texcoords.reserve(900000);

//   std::string line;
//   while (std::getline(file, line)) {
//     line = TrimView(line);
//     if (line.empty() || line[0] == '#') {
//       continue;
//     }

//     std::istringstream iss(line);
//     std::vector<std::string_view> tokens = Tokenize(line);
//     // std::string token;
//     // while (iss >> token) {
//     //   tokens.push_back(token);
//     // }

//     if (tokens.empty()) {
//       continue;
//     }

//     const std::string_view& keyword = tokens[0];

//     if (keyword == "v") {
//       ParseVertex(tokens);
//     } else if (keyword == "vn") {
//       ParseNormal(tokens);
//     } else if (keyword == "vt") {
//       ParseTexCoord(tokens);
//     } else if (keyword == "o") {
//       current_object = HandleObject(tokens);
//     } else if (keyword == "usemtl") {
//       current_mesh = HandleUseMtl(tokens, current_object);
//     } else if (keyword == "f") {
//       HandleFace(tokens, current_object, current_mesh);
//     }
//   }

//   file.close();
// }

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

void OBJData::parse(const std::string& filename) {
  // Memory mapping
  int fd = open(filename.c_str(), O_RDONLY);
  size_t size = lseek(fd, 0, SEEK_END);
  char* buffer =
      static_cast<char*>(mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0));
  close(fd);

  // Batch reserves
  vertices.reserve(1'000'000);
  normals.reserve(500'000);
  texcoords.reserve(500'000);
  objects.reserve(100);

  // Process buffer
  const char* current = buffer;
  const char* end = buffer + size;
  while (current < end) {
    const char* line_start = current;
    while (current < end && *current != '\n' && *current != '\r') ++current;
    std::string_view line(line_start, current - line_start);

    // Skip CRLF/LF
    while (current < end && (*current == '\r' || *current == '\n')) ++current;

    ProcessLine(line);
  }

  munmap(buffer, size);
}

std::string OBJData::Trim(const std::string& str) {
  size_t start = str.find_first_not_of(" \t");
  size_t end = str.find_last_not_of(" \t");
  return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

void OBJData::ParseVertex(const std::vector<std::string_view>& tokens) {
  if (tokens.size() < 4) {
    return;
  }
  // Assuming the vertex coordinates are in the format "v x y z".
  vertices.emplace_back(ParseFloat(tokens[1]), ParseFloat(tokens[2]),
                        ParseFloat(tokens[3]));
}

void OBJData::ParseNormal(const std::vector<std::string_view>& tokens) {
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
  // for (size_t i = 1; i < tokens.size(); ++i) {
  //   std::vector<std::string_view> vertex_indexes = SplitView(tokens[i], '/');
  //   VertexIndices vi;

  //   if (!vertex_indexes[0].empty()) {
  //     vi.v = ParseIndex(vertex_indexes[0], vertices.size());
  //   }
  //   if (vertex_indexes.size() > 1 && !vertex_indexes[1].empty()) {
  //     vi.vt = ParseIndex(vertex_indexes[1], texcoords.size());
  //   }
  //   if (vertex_indexes.size() > 2 && !vertex_indexes[2].empty()) {
  //     vi.vn = ParseIndex(vertex_indexes[2], normals.size());
  //   }

  //   face.vertices.push_back(vi);
  // }
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
    std::cerr << "Index out of bounds: " << part << std::endl;
    return -1;
  }
  return idx;
}

std::vector<std::string> OBJData::Split(const std::string_view& str,
                                        char delimiter) {
  std::vector<std::string> parts;
  std::stringstream ss(std::string{str});
  std::string part;
  while (std::getline(ss, part, delimiter)) {
    parts.push_back(part);
  }
  return parts;
}

}  // namespace s21