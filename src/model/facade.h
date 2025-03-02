#pragma once

#include <functional>
#include <tuple>

#include "filereader.h"
#include "scene.h"
#include "scene_parameters.h"

namespace s21 {

/**
 * @class Facade
 * @brief Singleton class that provides a unified interface for scene
 * management.
 *
 * The Facade class serves as a central point of interaction between the user
 * and the underlying components responsible for file reading, scene data
 * management, and transformation parameters. It employs the singleton pattern
 * to ensure that only one instance exists throughout the application's
 * lifetime.
 *
 * @note This class is non-copyable and non-movable to enforce the singleton
 * design.
 */
class Facade {
 public:
  /**
   * @typedef SceneUpdateCallback
   * @brief Callback function type for scene updates.
   *
   * This callback is invoked whenever the scene data is updated, passing a
   * shared pointer to the updated scene data.
   */
  using SceneUpdateCallback =
      std::function<void(const std::shared_ptr<DrawSceneData>&)>;

  // Deleted copy and move constructors and assignment operator to prevent
  // copying/moving
  Facade(const Facade& other) = delete;
  Facade(Facade&& other) = delete;
  void operator=(const Facade& other) = delete;

  /**
   * @brief Retrieves the singleton instance of the Facade class.
   * @return A shared pointer to the singleton Facade instance.
   *
   * This method provides access to the single instance of the Facade, creating
   * it on the first call.
   */
  static std::shared_ptr<Facade> GetInstance();

  /**
   * @brief Default destructor.
   *
   * No special cleanup is performed as resource management is handled by smart
   * pointers.
   */
  ~Facade() = default;

  /**
   * @brief Sets the callback function to be invoked when the scene data
   * updates.
   * @param callback The callback function to set.
   *
   * The provided callback will be called whenever the scene is modified,
   * passing the updated scene data.
   */
  void SetSceneUpdateCallback(SceneUpdateCallback callback);

  /**
   * @brief Loads a scene from the specified file path.
   * @param path The file path to the scene file (e.g., an OBJ file).
   * @return A shared pointer to the loaded scene data.
   *
   * This method utilizes the FileReader to parse the file and the Scene class
   * to process the data into a format suitable for rendering.
   */
  std::shared_ptr<DrawSceneData> LoadScene(const char* path);

  /**
   * @brief Resets the scene's transformation parameters to their default
   * values.
   *
   * This method resets the scale, rotation, and translation parameters,
   * effectively returning the scene to its initial position and orientation.
   */
  void resetScenePosition();

  /**
   * @brief Scales the scene uniformly along all axes.
   * @param value The scaling factor to apply.
   *
   * Adjusts the scene's scale uniformly and applies the transformation.
   */
  void Scale(const float value);

  /**
   * @brief Scales the scene along the X-axis.
   * @param value The scaling factor to apply along the X-axis.
   *
   * Modifies the X-axis scale parameter and updates the scene accordingly.
   */
  void ScaleX(const float value);

  /**
   * @brief Scales the scene along the Y-axis.
   * @param value The scaling factor to apply along the Y-axis.
   *
   * Modifies the Y-axis scale parameter and updates the scene accordingly.
   */
  void ScaleY(const float value);

  /**
   * @brief Scales the scene along the Z-axis.
   * @param value The scaling factor to apply along the Z-axis.
   *
   * Modifies the Z-axis scale parameter and updates the scene accordingly.
   */
  void ScaleZ(const float value);

  /**
   * @brief Moves the scene along the X-axis.
   * @param value The translation value along the X-axis.
   *
   * Adjusts the X-axis translation parameter and updates the scene's position.
   */
  void MoveX(const float value);

  /**
   * @brief Moves the scene along the Y-axis.
   * @param value The translation value along the Y-axis.
   *
   * Adjusts the Y-axis translation parameter and updates the scene's position.
   */
  void MoveY(const float value);

  /**
   * @brief Moves the scene along the Z-axis.
   * @param value The translation value along the Z-axis.
   *
   * Adjusts the Z-axis translation parameter and updates the scene's position.
   */
  void MoveZ(const float value);

  /**
   * @brief Rotates the scene around the X-axis.
   * @param value The rotation angle in degrees around the X-axis.
   *
   * Adjusts the X-axis rotation parameter and applies the transformation to the
   * scene.
   */
  void RotateX(const float value);

  /**
   * @brief Rotates the scene around the Y-axis.
   * @param value The rotation angle in degrees around the Y-axis.
   *
   * Adjusts the Y-axis rotation parameter and applies the transformation to the
   * scene.
   */
  void RotateY(const float value);

  /**
   * @brief Rotates the scene around the Z-axis.
   * @param value The rotation angle in degrees around the Z-axis.
   *
   * Adjusts the Z-axis rotation parameter and applies the transformation to the
   * scene.
   */
  void RotateZ(const float value);

  /**
   * @brief Retrieves the current transformation parameters of the scene.
   * @return A tuple containing the transformation parameters in the following
   * order:
   *         - Scale X
   *         - Scale Y
   *         - Scale Z
   *         - Rotation X (in degrees)
   *         - Rotation Y (in degrees)
   *         - Rotation Z (in degrees)
   *         - Translation X
   *         - Translation Y
   *         - Translation Z
   *
   * Provides access to the current state of the scene's transformations.
   */
  std::tuple<float, float, float, float, float, float, float, float, float>
  GetSceneParameters();

 private:
  std::unique_ptr<FileReader>
      fileReader_;  ///< Manages file reading operations (e.g., OBJ files).
  std::unique_ptr<Scene>
      scene_;  ///< Handles the scene data and its processing.
  std::unique_ptr<SceneParameters>
      sceneParam_;  ///< Stores the scene's transformation parameters.
  std::shared_ptr<DrawSceneData>
      currentSceneData_;  ///< Holds the current scene data for rendering.
  SceneUpdateCallback
      sceneUpdateCallback_;  ///< Callback invoked on scene updates.

  /**
   * @brief Private constructor to enforce singleton pattern.
   *
   * Initializes the facade's internal components. Accessible only through
   * GetInstance().
   */
  Facade();

  /**
   * @brief Applies the current transformation parameters to the scene.
   *
   * Uses the SceneParameters to compute and apply a transformation matrix to
   * the scene, updating the currentSceneData_ accordingly.
   */
  void TransformScene();
};
}  // namespace s21