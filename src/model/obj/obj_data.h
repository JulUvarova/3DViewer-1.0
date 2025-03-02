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

/**
 * @namespace s21
 * @brief Contains classes and structures for parsing and managing OBJ file
 * data.
 */
namespace s21 {

// --------------- Structures ---------------

/**
 * @struct VertexIndices
 * @brief Represents the indices for a vertex in a face, including vertex,
 * texture coordinate, and normal indices.
 *
 * This structure holds the indices referencing the vertices, texture
 * coordinates, and normals defined in the OBJ file. An index of -1 indicates
 * that the corresponding data is not present.
 */
struct VertexIndices {
  int v{-1};   ///< Index of the vertex, -1 if not specified.
  int vt{-1};  ///< Index of the texture coordinate, -1 if not specified.
  int vn{-1};  ///< Index of the normal, -1 if not specified.
  // add ctor
  explicit VertexIndices(int v, int vt = -1, int vn = -1)
      : v(v), vt(vt), vn(vn) {}
};

/**
 * @struct Face
 * @brief Represents a face in a mesh, consisting of multiple vertex indices.
 *
 * A face is defined by a collection of VertexIndices, each specifying the
 * vertex, texture coordinate, and normal for that point in the face.
 */
struct Face {
  std::vector<VertexIndices>
      vertices;  ///< List of vertex indices defining the face.
};

/**
 * @struct Mesh
 * @brief Represents a mesh within an object, associated with a material and
 * consisting of multiple faces.
 *
 * A mesh groups faces that share the same material.
 */
struct Mesh {
  std::string material;     ///< Name of the material used for this mesh.
  std::vector<Face> faces;  ///< List of faces that make up the mesh.
};

/**
 * @struct Object
 * @brief Represents an object in the OBJ file, which can contain multiple
 * meshes.
 *
 * An object is a collection of meshes, each potentially using different
 * materials.
 */
struct Object {
  std::string name;          ///< Name of the object.
  std::vector<Mesh> meshes;  ///< List of meshes that make up the object.
};

// --------------- Class OBJData ---------------

/**
 * @class OBJData
 * @brief Manages the parsing and storage of data from an OBJ file.
 *
 * This class handles the reading and processing of OBJ files, storing the
 * geometric data such as vertices, texture coordinates, normals, and the
 * hierarchical structure of objects and meshes. It also provides functionality
 * to normalize the vertex data and generate a string representation of the
 * parsed data.
 */
class OBJData {
 public:
  std::vector<Vec3f> vertices;   ///< List of 3D vertices.
  std::vector<Vec2f> texcoords;  ///< List of 2D texture coordinates.
  std::vector<Vec3f> normals;    ///< List of 3D normals.
  std::vector<Object> objects;   ///< List of objects parsed from the file.
  float x_min = 0.0f, x_max = 0.0f, y_min = 0.0f, y_max = 0.0f, z_min = 0.0f,
        z_max = 0.0f;
  ///< Bounding box of the vertex data.

  /**
   * @brief Parses an OBJ file and populates the data structures.
   * @param filename The path to the OBJ file to parse.
   *
   * This method reads the specified OBJ file, processes its contents, and fills
   * the vertices, texcoords, normals, and objects vectors accordingly. Any
   * parsing errors are collected and can be accessed via the errors_ vector.
   */
  void Parse(const std::string& filename);

  /**
   * @brief Normalizes the vertex data to fit within a unit cube.
   *
   * This method adjusts the vertex coordinates so that the entire model fits
   * within a cube ranging from (-1, -1, -1) to (1, 1, 1), based on the bounding
   * box defined by x_min, x_max, y_min, y_max, z_min, and z_max.
   */
  void Normalize();

  /**
   * @brief Generates a string representation of the parsed data.
   * @return A string containing information about the parsed objects, meshes,
   * and faces.
   *
   * Useful for debugging or logging purposes.
   */
  std::string toString();

 private:
  Object* current_object_ =
      nullptr;  ///< Pointer to the current object being processed.
  Mesh* current_mesh_ =
      nullptr;  ///< Pointer to the current mesh being processed.
  std::vector<std::string>
      errors_;  ///< List of errors encountered during parsing.

  /**
   * @brief Processes a single line from the OBJ file.
   * @param line The line to process.
   *
   * Determines the type of data represented by the line (e.g., vertex, face,
   * object) and calls the appropriate parsing method.
   */
  void ProcessLine(std::string_view line);

  /**
   * @brief Parses a vertex line and adds the vertex to the vertices vector.
   * @param tokens The tokenized parts of the line.
   */
  void ParseVertex(const std::vector<std::string_view>& tokens);

  /**
   * @brief Parses a normal line and adds the normal to the normals vector.
   * @param tokens The tokenized parts of the line.
   */
  void ParseNormal(const std::vector<std::string_view>& tokens);

  /**
   * @brief Parses a texture coordinate line and adds it to the texcoords
   * vector.
   * @param tokens The tokenized parts of the line.
   */
  void ParseTexCoord(const std::vector<std::string_view>& tokens);

  /**
   * @brief Handles the creation or selection of an object based on the 'o'
   * command.
   * @param tokens The tokenized parts of the line.
   * @return A pointer to the current object.
   */
  Object* HandleObject(const std::vector<std::string_view>& tokens);

  /**
   * @brief Handles the 'usemtl' command to associate a material with the
   * current mesh.
   * @param tokens The tokenized parts of the line.
   * @param current_object Pointer to the current object.
   * @return A pointer to the current mesh.
   */
  Mesh* HandleUseMtl(const std::vector<std::string_view>& tokens,
                     Object* current_object);

  /**
   * @brief Parses a face line and adds the face to the current mesh.
   * @param tokens The tokenized parts of the line.
   * @param current_object Reference to the pointer of the current object.
   * @param current_mesh Reference to the pointer of the current mesh.
   */
  void HandleFace(const std::vector<std::string_view>& tokens,
                  Object*& current_object, Mesh*& current_mesh);

  /**
   * @brief Parses an index from a string view, handling negative indices.
   * @param part The string view containing the index.
   * @param current_count The current number of elements (vertices, texcoords,
   * etc.).
   * @return The parsed index, adjusted for negative values.
   */
  int ParseIndex(const std::string_view& part, size_t current_count);

  /**
   * @brief Parses a float from a string view.
   * @param sv The string view to parse.
   * @return The parsed float value.
   */
  float ParseFloat(std::string_view sv);

  /**
   * @brief Trims whitespace from the beginning and end of a string view.
   * @param sv The string view to trim.
   * @return A trimmed string view.
   */
  std::string_view TrimView(std::string_view sv);

  /**
   * @brief Tokenizes a line into words, ignoring comments and trimming
   * whitespace.
   * @param line The line to tokenize.
   * @return A vector of string views representing the tokens.
   */
  std::vector<std::string_view> Tokenize(std::string_view line);
};
}  // namespace s21