#pragma once

#include <iostream>

#include "../model/facade.h"
#include "../model/scene.h"
#include "../model/scene_parameters.h"

namespace s21 {

class Controller {
 private:
  Facade* facade_;

  const float kScaleCorrection = 100.0;
  const float kRotationCorrection = 1.0;
  const float kMoveCorrection = 200.0;

 public:
  Controller() { facade_ = new Facade(); }

  ~Controller() { delete facade_; }

  // Upload scene
  DrawSceneData LoadScene(const char* filename) {
    return facade_->LoadScene(filename);
  }

  DrawSceneData ResetScene() { return facade_->resetScenePosition(); }

  // Setters
  DrawSceneData SetScaleX(const int value) {
    return facade_->Scale(value / kScaleCorrection);
  }

  DrawSceneData SetScaleY(const int value) {
    return facade_->Scale(value / kScaleCorrection);
  }

  DrawSceneData SetScaleZ(const int value) {
    return facade_->Scale(value / kScaleCorrection);
  }

  DrawSceneData SetLocationX(const int value) {
    return facade_->MoveX(value / kMoveCorrection);
  }

  DrawSceneData SetLocationY(const int value) {
    return facade_->MoveY(value / kMoveCorrection);
  }

  DrawSceneData SetLocationZ(const int value) {
    return facade_->MoveZ(value / kMoveCorrection);
  }

  DrawSceneData SetRotationX(const int value) {
    return facade_->RotateX(value / kRotationCorrection);
  }

  DrawSceneData SetRotationY(const int value) {
    return facade_->RotateY(value / kRotationCorrection);
  }

  DrawSceneData SetRotationZ(const int value) {
    return facade_->RotateZ(value / kRotationCorrection);
  }
};
}  // namespace s21
