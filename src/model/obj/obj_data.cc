#include "obj_data.h"

namespace s21 {

// Reusable helper function to parse a float from a string.
// Tries std::from_chars first, falling back to std::stof if needed.
inline float ParseFloat(const std::string& s) {
  float value = 0.0f;
  const char* begin = s.data();
  const char* end = begin + s.size();
  auto result = std::from_chars(begin, end, value);
  if (result.ec != std::errc()) {
    // Fallback conversion using std::stof if from_chars fails.
    value = std::stof(s);
  }
  return value;
}

void OBJData::parse(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return;
  }

  Object* current_object = nullptr;
  Mesh* current_mesh = nullptr;

  std::string line;
  while (std::getline(file, line)) {
    line = Trim(line);
    if (line.empty() || line[0] == '#') {
      continue;
    }

    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
      tokens.push_back(token);
    }

    if (tokens.empty()) {
      continue;
    }

    const std::string& keyword = tokens[0];

    if (keyword == "v") {
      ParseVertex(tokens);
    } else if (keyword == "vn") {
      ParseNormal(tokens);
    } else if (keyword == "vt") {
      ParseTexCoord(tokens);
    } else if (keyword == "o") {
      current_object = HandleObject(tokens);
    } else if (keyword == "usemtl") {
      current_mesh = HandleUseMtl(tokens, current_object);
    } else if (keyword == "f") {
      HandleFace(tokens, current_object, current_mesh);
    }
  }

  file.close();
}

std::string OBJData::Trim(const std::string& str) {
  size_t start = str.find_first_not_of(" \t");
  size_t end = str.find_last_not_of(" \t");
  return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

void OBJData::ParseVertex(const std::vector<std::string>& tokens) {
  if (tokens.size() < 4) {
    return;
  }
  // Assuming the vertex coordinates are in the format "v x y z".
  vertices.emplace_back(ParseFloat(tokens[1]), ParseFloat(tokens[2]),
                        ParseFloat(tokens[3]));
}

void OBJData::ParseNormal(const std::vector<std::string>& tokens) {
  if (tokens.size() < 4) {
    return;
  }
  // Assuming the vertex coordinates are in the format "vn x y z".
  normals.emplace_back(ParseFloat(tokens[1]), ParseFloat(tokens[2]),
                       ParseFloat(tokens[3]));
}

void OBJData::ParseTexCoord(const std::vector<std::string>& tokens) {
  if (tokens.size() < 3) {
    return;
  }
  // Assuming the texture coordinates are in the format "vt u v".
  texcoords.emplace_back(ParseFloat(tokens[1]), ParseFloat(tokens[2]));
}

Object* OBJData::HandleObject(const std::vector<std::string>& tokens) {
  if (tokens.size() < 2) {
    return nullptr;
  }
  objects.emplace_back();
  Object& obj = objects.back();
  obj.name = tokens[1];
  return &obj;
}

Mesh* OBJData::HandleUseMtl(const std::vector<std::string>& tokens,
                            Object* current_object) {
  if (tokens.size() < 2 || !current_object) {
    return nullptr;
  }
  std::string material = tokens[1];
  if (current_object->meshes.empty() ||
      current_object->meshes.back().material != material) {
    current_object->meshes.emplace_back();
    current_object->meshes.back().material = material;
  }
  return &current_object->meshes.back();
}

void OBJData::HandleFace(const std::vector<std::string>& tokens,
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
    std::vector<std::string> vertex_indexes = Split(tokens[i], '/');
    VertexIndices vi;

    if (!vertex_indexes[0].empty()) {
      vi.v = ParseIndex(vertex_indexes[0], vertices.size());
    }
    if (vertex_indexes.size() > 1 && !vertex_indexes[1].empty()) {
      vi.vt = ParseIndex(vertex_indexes[1], texcoords.size());
    }
    if (vertex_indexes.size() > 2 && !vertex_indexes[2].empty()) {
      vi.vn = ParseIndex(vertex_indexes[2], normals.size());
    }

    face.vertices.push_back(vi);
  }
  current_mesh->faces.push_back(face);
}

int OBJData::ParseIndex(const std::string& part, size_t current_count) {
  int idx = std::stoi(part);
  if (idx < 0) {
    idx += current_count;
  } else {
    idx -= 1;
  }
  if (idx < 0 || idx >= static_cast<int>(current_count)) {
    std::cerr << "Index out of bounds: " << part << std::endl;
    return -1;
  }
  return idx;
}

std::vector<std::string> OBJData::Split(const std::string& str,
                                        char delimiter) {
  std::vector<std::string> parts;
  std::stringstream ss(str);
  std::string part;
  while (std::getline(ss, part, delimiter)) {
    parts.push_back(part);
  }
  return parts;
}

}  // namespace s21