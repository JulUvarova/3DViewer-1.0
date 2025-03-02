#include "controller.h"
namespace s21 {
Controller::Controller() {
  facade_ = Facade::GetInstance();

  // Set the callback in the facade
  facade_->SetSceneUpdateCallback(
      [this](const std::shared_ptr<DrawSceneData> &sceneData) {
        if (sceneUpdateCallback_) {
          sceneUpdateCallback_(sceneData);
        }
      });
}

std::shared_ptr<Controller> Controller::GetInstance() {
  static auto instance = std::shared_ptr<Controller>(new Controller);
  return instance;
}

void Controller::SetSceneUpdateCallback(
    std::function<void(const std::shared_ptr<DrawSceneData> &)> callback) {
  sceneUpdateCallback_ = callback;
}

std::shared_ptr<DrawSceneData> Controller::LoadScene(const char *filename) {
  return facade_->LoadScene(filename);
}

void Controller::ResetScene() { facade_->resetScenePosition(); }

void Controller::SetScaleX(const int value) {
  facade_->Scale(value / kScaleCorrection);
}

void Controller::SetScaleY(const int value) {
  facade_->Scale(value / kScaleCorrection);
}

void Controller::SetScaleZ(const int value) {
  facade_->Scale(value / kScaleCorrection);
}

void Controller::SetLocationX(const int value) {
  facade_->MoveX(value / kMoveCorrection);
}

void Controller::SetLocationY(const int value) {
  facade_->MoveY(value / kMoveCorrection);
}

void Controller::SetLocationZ(const int value) {
  facade_->MoveZ(value / kMoveCorrection);
}

void Controller::SetRotationX(const int value) {
  facade_->RotateX(value / kRotationCorrection);
}

void Controller::SetRotationY(const int value) {
  facade_->RotateY(value / kRotationCorrection);
}

void Controller::SetRotationZ(const int value) {
  facade_->RotateZ(value / kRotationCorrection);
}

std::tuple<float, float, float, float, float, float, float, float, float>
Controller::GetSceneParameters() {
  return facade_->GetSceneParameters();
}
}  // namespace s21