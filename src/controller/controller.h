#pragma once

#include <iostream>
#include <memory>
#include <functional>

#include "../model/facade.h"
#include "../model/scene.h"
#include "../model/scene_parameters.h"

namespace s21 {

class Controller {
 private:
  std::unique_ptr<Facade> facade_;

  const float kScaleCorrection = 100.0;
  const float kRotationCorrection = 1.0;
  const float kMoveCorrection = 200.0;

  // Callback for scene updates
  std::function<void(const std::shared_ptr<DrawSceneData>&)> sceneUpdateCallback_;

 public:
  Controller() {
    facade_ = std::make_unique<Facade>();
    
    // Set the callback in the facade
    facade_->SetSceneUpdateCallback([this](const std::shared_ptr<DrawSceneData>& sceneData) {
      if (sceneUpdateCallback_) {
        sceneUpdateCallback_(sceneData);
      }
    });
  }

  ~Controller() { }

  // Set callback for scene updates
  void SetSceneUpdateCallback(std::function<void(const std::shared_ptr<DrawSceneData>&)> callback) {
    sceneUpdateCallback_ = callback;
  }

  // Upload scene
  std::shared_ptr<DrawSceneData> LoadScene(const char* filename) {
    return facade_->LoadScene(filename);
  }

  void ResetScene() { facade_->resetScenePosition(); }

  // Setters
  void SetScaleX(const int value) { facade_->Scale(value / kScaleCorrection); }

  void SetScaleY(const int value) { facade_->Scale(value / kScaleCorrection); }

  void SetScaleZ(const int value) { facade_->Scale(value / kScaleCorrection); }

  void SetLocationX(const int value) {
    facade_->MoveX(value / kMoveCorrection);
  }

  void SetLocationY(const int value) {
    facade_->MoveY(value / kMoveCorrection);
  }

  void SetLocationZ(const int value) {
    facade_->MoveZ(value / kMoveCorrection);
  }

  void SetRotationX(const int value) {
    facade_->RotateX(value / kRotationCorrection);
  }

  void SetRotationY(const int value) {
    facade_->RotateY(value / kRotationCorrection);
  }

  void SetRotationZ(const int value) {
    facade_->RotateZ(value / kRotationCorrection);
  }
};
}  // namespace s21
