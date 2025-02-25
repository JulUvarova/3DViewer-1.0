#include "facade.h"

// std::shared_ptr<s21::Facade> s21::Facade::instance_ = nullptr;
// std::once_flag s21::Facade::initFlag_;

s21::Facade::Facade() {
  fileReader_ = std::make_unique<FileReader>();
  sceneParam_ = std::make_unique<SceneParameters>();
}

std::shared_ptr<s21::DrawSceneData> s21::Facade::LoadScene(const char *path) {
  scene_.reset();
  scene_ = std::make_unique<s21::Scene>();
  auto sceneData = scene_->LoadSceneMeshData(fileReader_->ReadFile(path));

  // Store the initial scene data
  if (sceneData) {
    currentSceneData_ = sceneData;
  }

  return sceneData;
}

void s21::Facade::resetScenePosition() {
  if (!scene_) return;

  sceneParam_ = std::make_unique<SceneParameters>();
  TransformScene();
}

void s21::Facade::Scale(const float value) {
  if (!scene_) return;

  sceneParam_->SetScaleX(value);
  sceneParam_->SetScaleY(value);
  sceneParam_->SetScaleZ(value);
  TransformScene();
}

void s21::Facade::ScaleX(const float value) {
  if (!scene_) return;

  sceneParam_->SetScaleX(value);
  TransformScene();
}

void s21::Facade::ScaleY(const float value) {
  if (!scene_) return;

  sceneParam_->SetScaleY(value);
  TransformScene();
}

void s21::Facade::ScaleZ(const float value) {
  if (!scene_) return;

  sceneParam_->SetScaleZ(value);
  TransformScene();
}

void s21::Facade::MoveX(const float value) {
  if (!scene_) return;

  sceneParam_->SetLocationX(value);
  TransformScene();
}

void s21::Facade::MoveY(const float value) {
  if (!scene_) return;

  sceneParam_->SetLocationY(value);
  TransformScene();
}

void s21::Facade::MoveZ(const float value) {
  if (!scene_) return;

  sceneParam_->SetLocationZ(value);
  TransformScene();
}

void s21::Facade::RotateX(const float value) {
  if (!scene_) return;

  sceneParam_->SetRotationX(value);
  TransformScene();
}

void s21::Facade::RotateY(const float value) {
  if (!scene_) return;

  sceneParam_->SetRotationY(value);
  TransformScene();
}

void s21::Facade::RotateZ(const float value) {
  if (!scene_) return;

  sceneParam_->SetRotationZ(value);
  TransformScene();
}

void s21::Facade::TransformScene() {
  Mat4f move_mat = TransformMatrixBuilder::CreateMoveMatrix(
      sceneParam_->GetLocationX(), sceneParam_->GetLocationY(),
      sceneParam_->GetLocationZ());
  Mat4f rotate_mat = TransformMatrixBuilder::CreateRotationMatrix(
      sceneParam_->GetRotationX(), sceneParam_->GetRotationY(),
      sceneParam_->GetRotationZ());
  Mat4f scale_mat = TransformMatrixBuilder::CreateScaleMatrix(
      sceneParam_->GetScaleX(), sceneParam_->GetScaleY(),
      sceneParam_->GetScaleZ());
  Mat4f transform_mat = scale_mat * move_mat * rotate_mat;
  scene_->TransformSceneMeshData(transform_mat);

  // Notify about the update if callback is set
  if (sceneUpdateCallback_ && currentSceneData_) {
    sceneUpdateCallback_(currentSceneData_);
  }
}