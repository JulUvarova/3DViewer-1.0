#include "controller.h"

std::shared_ptr<s21::Controller> s21::Controller::instance_ = nullptr;
std::once_flag s21::Controller::initFlag_;

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