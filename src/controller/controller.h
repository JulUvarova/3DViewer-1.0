#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <mutex>

#include "../model/facade.h"
#include "../model/scene.h"
#include "../model/scene_parameters.h"

namespace s21 {

class Controller {
 public:
  Controller(const Controller &other) = delete;
  Controller(Controller &&other) = delete;
  void operator=(const Controller &other) = delete;
  ~Controller() {
    LogInfoOnce << "Delete controller" << std::endl;
  };

  static std::shared_ptr<Controller> GetInstance() {
    std::call_once(initFlag_, []() {
      instance_.reset(new Controller());
      LogInfoOnce << "Create controller" << std::endl;
    });
    return instance_;
  }

  // Set callback for scene updates
  inline void SetSceneUpdateCallback(
      std::function<void(const std::shared_ptr<DrawSceneData> &)> callback) {
    sceneUpdateCallback_ = callback;
  }

  // Upload scene
  inline std::shared_ptr<DrawSceneData> LoadScene(const char *filename) {
    return facade_->LoadScene(filename);
  }

  inline void ResetScene() { facade_->resetScenePosition(); }

  // Setters
  inline void SetScaleX(const int value) {
    facade_->Scale(value / kScaleCorrection);
  }

  inline void SetScaleY(const int value) {
    facade_->Scale(value / kScaleCorrection);
  }

  inline void SetScaleZ(const int value) {
    facade_->Scale(value / kScaleCorrection);
  }

  inline void SetLocationX(const int value) {
    facade_->MoveX(value / kMoveCorrection);
  }

  inline void SetLocationY(const int value) {
    facade_->MoveY(value / kMoveCorrection);
  }

  inline void SetLocationZ(const int value) {
    facade_->MoveZ(value / kMoveCorrection);
  }

  inline void SetRotationX(const int value) {
    facade_->RotateX(value / kRotationCorrection);
  }

  inline void SetRotationY(const int value) {
    facade_->RotateY(value / kRotationCorrection);
  }

  inline void SetRotationZ(const int value) {
    facade_->RotateZ(value / kRotationCorrection);
  }

 private:
  std::shared_ptr<Facade> facade_;
  static std::shared_ptr<Controller> instance_;
  static std::once_flag initFlag_;

  const float kScaleCorrection = 100.0;
  const float kRotationCorrection = 1.0;
  const float kMoveCorrection = 200.0;

  // Callback for scene updates
  std::function<void(const std::shared_ptr<DrawSceneData> &)>
      sceneUpdateCallback_;

  Controller();
};
}  // namespace s21
