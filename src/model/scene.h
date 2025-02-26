#pragma once

#include <algorithm>
#include <thread>
#include <vector>

#include "obj/obj_data.h"

namespace s21 {

/**
 * @struct DrawSceneData
 * @brief Holds the data required for rendering the scene, including vertices,
 * indices, and metadata.
 *
 * This structure encapsulates the essential data needed to draw a 3D scene. It
 * includes vertex coordinates, indices for constructing the mesh topology, and
 * a string for additional scene information.
 */
struct DrawSceneData {
  std::vector<float> vertices;      ///< Vertex coordinates stored as floats,
                                    ///< representing the mesh geometry.
  std::vector<int> vertex_indices;  ///< Indices that define the mesh topology
                                    ///< by connecting vertices.
  std::string info;  ///< Additional metadata or information about the scene.
};

/**
 * @class Scene
 * @brief Manages the loading and transformation of mesh data for a 3D scene.
 *
 * The `Scene` class is designed to handle mesh data operations, such as loading
 * from an `OBJData` object and applying transformations to the mesh vertices.
 * It maintains a private vector of vertices in homogeneous coordinates and a
 * shared pointer to rendering data.
 */
class Scene {
 public:
  /**
   * @brief Loads mesh data from an `OBJData` object into a format suitable for
   * rendering.
   * @param obj_data The `OBJData` object containing the raw mesh data to be
   * processed.
   * @return A shared pointer to a `DrawSceneData` object containing the
   * prepared mesh data.
   *
   * This method extracts mesh information from the provided `OBJData` object
   * and organizes it into a `DrawSceneData` structure, which includes vertices,
   * indices, and metadata for rendering.
   */
  std::shared_ptr<DrawSceneData> LoadSceneMeshData(OBJData obj_data);

  /**
   * @brief Applies a transformation matrix to the scene's mesh vertices.
   * @param transform_matrix A 4x4 transformation matrix (Mat4f) to modify the
   * mesh geometry.
   *
   * This method transforms each vertex in the mesh by applying the specified
   * 4x4 transformation matrix, which can represent operations such as
   * translation, rotation, or scaling. The transformed vertices are stored
   * internally for further use.
   */
  void TransformSceneMeshData(Mat4f& transform_matrix);

 private:
  std::vector<Vec4f> mesh_vertexes_;  ///< Mesh vertices stored as 4D vectors in
                                      ///< homogeneous coordinates.
  std::shared_ptr<DrawSceneData>
      draw_scene_data_;  ///< Shared pointer to the rendering data of the scene.
};
}  // namespace s21