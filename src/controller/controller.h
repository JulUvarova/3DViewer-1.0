#pragma once

#include <functional>
#include <iostream>
#include <memory>

#include "facade.h"
#include "scene.h"
#include "scene_parameters.h"

namespace s21 {

/**
 * @brief The Controller class provides an interface for manipulating and
 * updating a 3D scene.
 *
 * The Controller acts as a singleton facade that connects the user interface
 * with the underlying 3D scene operations. It allows for loading scenes,
 * applying transformations such as scaling, translation, and rotation, and
 * setting update callbacks for when the scene changes.
 */
class Controller {
 public:
  /// Deleted copy constructor to enforce singleton behavior.
  Controller(const Controller &other) = delete;
  /// Deleted move constructor to enforce singleton behavior.
  Controller(Controller &&other) = delete;
  /// Deleted copy assignment operator to enforce singleton behavior.
  void operator=(const Controller &other) = delete;
  /// Default destructor.
  ~Controller() = default;

  /**
   * @brief Retrieves the singleton instance of the Controller.
   *
   * This static method provides access to the unique Controller instance,
   * creating it upon the first call.
   *
   * @return std::shared_ptr<Controller> A shared pointer to the singleton
   * instance.
   */
  static std::shared_ptr<Controller> GetInstance();

  /**
   * @brief Sets the callback function to be invoked on scene updates.
   *
   * The provided callback is used to notify listeners when the scene is updated
   * and needs to be redrawn.
   *
   * @param callback A function taking a shared pointer to DrawSceneData.
   */
  void SetSceneUpdateCallback(
      std::function<void(const std::shared_ptr<DrawSceneData> &)> callback);

  /**
   * @brief Loads a scene from a specified file.
   *
   * This function delegates the scene loading process to the Facade, which
   * handles file parsing and scene creation.
   *
   * @param filename The file path of the scene to load.
   * @return std::shared_ptr<DrawSceneData> A shared pointer to the loaded scene
   * data.
   */
  std::shared_ptr<DrawSceneData> LoadScene(const char *filename);

  /**
   * @brief Resets the scene to its default position.
   *
   * This function calls the Facade to reinitialize the scene's position to a
   * default state.
   */
  void ResetScene();

  /**
   * @brief Sets the scaling factor along the X-axis.
   *
   * The provided value is adjusted by an internal scale correction factor
   * before applying.
   *
   * @param value The raw scaling factor for the X-axis.
   */
  void SetScaleX(const int value);

  /**
   * @brief Sets the scaling factor along the Y-axis.
   *
   * The provided value is adjusted by an internal scale correction factor
   * before applying.
   *
   * @param value The raw scaling factor for the Y-axis.
   */
  void SetScaleY(const int value);

  /**
   * @brief Sets the scaling factor along the Z-axis.
   *
   * The provided value is adjusted by an internal scale correction factor
   * before applying.
   *
   * @param value The raw scaling factor for the Z-axis.
   */
  void SetScaleZ(const int value);

  /**
   * @brief Sets the translation offset along the X-axis.
   *
   * The provided value is adjusted by an internal move correction factor before
   * applying.
   *
   * @param value The raw translation value for the X-axis.
   */
  void SetLocationX(const int value);

  /**
   * @brief Sets the translation offset along the Y-axis.
   *
   * The provided value is adjusted by an internal move correction factor before
   * applying.
   *
   * @param value The raw translation value for the Y-axis.
   */
  void SetLocationY(const int value);

  /**
   * @brief Sets the translation offset along the Z-axis.
   *
   * The provided value is adjusted by an internal move correction factor before
   * applying.
   *
   * @param value The raw translation value for the Z-axis.
   */
  void SetLocationZ(const int value);

  /**
   * @brief Sets the rotation angle around the X-axis.
   *
   * The provided value is adjusted by an internal rotation correction factor
   * before applying.
   *
   * @param value The raw rotation angle for the X-axis.
   */
  void SetRotationX(const int value);

  /**
   * @brief Sets the rotation angle around the Y-axis.
   *
   * The provided value is adjusted by an internal rotation correction factor
   * before applying.
   *
   * @param value The raw rotation angle for the Y-axis.
   */
  void SetRotationY(const int value);

  /**
   * @brief Sets the rotation angle around the Z-axis.
   *
   * The provided value is adjusted by an internal rotation correction factor
   * before applying.
   *
   * @param value The raw rotation angle for the Z-axis.
   */
  void SetRotationZ(const int value);

  /**
   * @brief Retrieves the current scene parameters.
   *
   * This function returns a tuple containing various parameters of the scene,
   * including scaling, rotation, and translation values.
   *
   * @return std::tuple<float, float, float, float, float, float, float, float,
   * float> A tuple of scene parameters.
   */
  std::tuple<float, float, float, float, float, float, float, float, float>
  GetSceneParameters();

 private:
  /// Pointer to the Facade that handles the low-level operations for the 3D
  /// scene.
  std::shared_ptr<Facade> facade_;

  /// Correction factor used to adjust raw scaling values.
  const float kScaleCorrection = 100.0;
  /// Correction factor used to adjust raw rotation values.
  const float kRotationCorrection = 1.0;
  /// Correction factor used to adjust raw movement values.
  const float kMoveCorrection = 200.0;

  /// Callback function to be executed when the scene is updated.
  std::function<void(const std::shared_ptr<DrawSceneData> &)>
      sceneUpdateCallback_;

  /**
   * @brief Private constructor to enforce singleton pattern.
   *
   * The constructor is private to prevent instantiation outside the
   * GetInstance() method.
   */
  Controller();
};

}  // namespace s21
