#pragma once

#include <functional>

#include "filereader.h"
#include "scene.h"
#include "scene_parameters.h"

namespace s21 {

class Facade {
 public:
  // Define a callback type for scene updates
  using SceneUpdateCallback =
      std::function<void(const std::shared_ptr<DrawSceneData> &)>;

  Facade(const Facade &other) = delete;
  Facade(Facade &&other) = delete;
  void operator=(const Facade &other) = delete;

  static std::shared_ptr<Facade> GetInstance() {
    if (!instance_) {
      instance_ = std::shared_ptr<Facade>(new Facade());
      LogInfoOnce << "Create facade" << std::endl;
    }
    return instance_;
  }

  ~Facade() { LogInfoOnce << "Delete facade" << std::endl; };

  // Set callback for scene updates
  inline void SetSceneUpdateCallback(SceneUpdateCallback callback) {
    sceneUpdateCallback_ = callback;
  }

  std::shared_ptr<DrawSceneData> LoadScene(const char *path);

  void resetScenePosition();

  void Scale(const float value);

  void ScaleX(const float value);

  void ScaleY(const float value);

  void ScaleZ(const float value);

  void MoveX(const float value);

  void MoveY(const float value);

  void MoveZ(const float value);

  void RotateX(const float value);

  void RotateY(const float value);

  void RotateZ(const float value);

 private:
  std::unique_ptr<FileReader> fileReader_;
  std::unique_ptr<Scene> scene_;
  std::unique_ptr<SceneParameters> sceneParam_;
  std::shared_ptr<DrawSceneData> currentSceneData_;
  SceneUpdateCallback sceneUpdateCallback_;
  static std::shared_ptr<Facade> instance_;

  Facade();

  void TransformScene();
};
}  // namespace s21
