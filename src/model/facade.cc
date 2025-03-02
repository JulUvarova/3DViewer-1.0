#include "facade.h"

namespace s21 {
Facade::Facade()
    : fileReader_(std::make_unique<FileReader>()),
      sceneParam_(std::make_unique<SceneParameters>()) {}

std::shared_ptr<DrawSceneData> Facade::LoadScene(const char *path) {
  scene_.reset();
  scene_ = std::make_unique<Scene>();
  auto sceneData = scene_->LoadSceneMeshData(fileReader_->ReadFile(path));

  // Store the initial scene data
  if (sceneData) {
    currentSceneData_ = sceneData;
  }

  return sceneData;
}

std::shared_ptr<Facade> Facade::GetInstance() {
  static auto instance = std::shared_ptr<Facade>(new Facade);
  return instance;
}

void Facade::SetSceneUpdateCallback(SceneUpdateCallback callback) {
  sceneUpdateCallback_ = callback;
}
void Facade::resetScenePosition() {
  if (!scene_) return;

  sceneParam_ = std::make_unique<SceneParameters>();
  TransformScene();
}

void Facade::Scale(const float value) {
  if (!scene_) return;

  sceneParam_->SetScaleX(value);
  sceneParam_->SetScaleY(value);
  sceneParam_->SetScaleZ(value);
  TransformScene();
}

void Facade::ScaleX(const float value) {
  if (!scene_) return;

  sceneParam_->SetScaleX(value);
  TransformScene();
}

void Facade::ScaleY(const float value) {
  if (!scene_) return;

  sceneParam_->SetScaleY(value);
  TransformScene();
}

void Facade::ScaleZ(const float value) {
  if (!scene_) return;

  sceneParam_->SetScaleZ(value);
  TransformScene();
}

void Facade::MoveX(const float value) {
  if (!scene_) return;

  sceneParam_->SetLocationX(value);
  TransformScene();
}

void Facade::MoveY(const float value) {
  if (!scene_) return;

  sceneParam_->SetLocationY(value);
  TransformScene();
}

void Facade::MoveZ(const float value) {
  if (!scene_) return;

  sceneParam_->SetLocationZ(value);
  TransformScene();
}

void Facade::RotateX(const float value) {
  if (!scene_) return;

  sceneParam_->SetRotationX(value);
  TransformScene();
}

void Facade::RotateY(const float value) {
  if (!scene_) return;

  sceneParam_->SetRotationY(value);
  TransformScene();
}

void Facade::RotateZ(const float value) {
  if (!scene_) return;

  sceneParam_->SetRotationZ(value);
  TransformScene();
}

std::tuple<float, float, float, float, float, float, float, float, float>
Facade::GetSceneParameters() {
  if (!scene_) return std::make_tuple(0, 0, 0, 0, 0, 0, 0, 0, 0);
  return std::make_tuple(
      sceneParam_->GetLocationX(), sceneParam_->GetLocationY(),
      sceneParam_->GetLocationZ(), sceneParam_->GetRotationX(),
      sceneParam_->GetRotationY(), sceneParam_->GetRotationZ(),
      sceneParam_->GetScaleX(), sceneParam_->GetScaleY(),
      sceneParam_->GetScaleZ());
}

void Facade::TransformScene() {
  // Notify about the update if callback is set
  if (sceneUpdateCallback_ && currentSceneData_) {
    sceneUpdateCallback_(currentSceneData_);
  }
}
}  // namespace s21