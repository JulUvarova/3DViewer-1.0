#include "controller.h"

s21::Controller::Controller() {
  facade_ = s21::Facade::GetInstance();

  // Set the callback in the facade
  facade_->SetSceneUpdateCallback(
      [this](const std::shared_ptr<DrawSceneData>& sceneData) {
        if (sceneUpdateCallback_) {
          sceneUpdateCallback_(sceneData);
        }
      });
}