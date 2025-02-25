#pragma once

#include "filereader.h"
#include "scene.h"
#include "scene_parameters.h"

namespace s21 {

class Facade {
 public:
  Facade() {
    fileReader_ = std::make_unique<FileReader>();
    sceneParam_ = std::make_unique<SceneParameters>();  }

  ~Facade() {
  }

  std::shared_ptr<DrawSceneData> LoadScene(const char *path) {
    scene_.reset();
    scene_ = std::make_unique<Scene>();
    // TODO проверка загрузилась ли сцена
    return scene_->LoadSceneMeshData(fileReader_->ReadFile(path));
  }

  void resetScenePosition() {
    if (!scene_) return;

    sceneParam_ = std::make_unique<SceneParameters>();  // Fix: Use make_unique
    TransformScene();
  }

  void Scale(const float value) {
    if (!scene_) return;

    sceneParam_->SetScaleX(value);
    TransformScene();
  }

  void ScaleX(const float value) {
    if (!scene_) return;

    sceneParam_->SetScaleX(value);
    TransformScene();
  }

  void ScaleY(const float value) {
    if (!scene_) return;

    sceneParam_->SetScaleX(value);
    TransformScene();
  }

  void ScaleZ(const float value) {
    if (!scene_) return;

    sceneParam_->SetScaleX(value);
    TransformScene();
  }

  void MoveX(const float value) {
    if (!scene_) return;

    sceneParam_->SetLocationX(value);
    TransformScene();
  }

  void MoveY(const float value) {
    if (!scene_) return;

    sceneParam_->SetLocationY(value);
    TransformScene();
  }

  void MoveZ(const float value) {
    if (!scene_) return;

    sceneParam_->SetLocationZ(value);
    TransformScene();
  }

  void RotateX(const float value) {
    if (!scene_) return;

    sceneParam_->SetRotationX(value);
    TransformScene();
  }

  void RotateY(const float value) {
    if (!scene_) return;

    sceneParam_->SetRotationY(value);
    TransformScene();
  }

  void RotateZ(const float value) {
    if (!scene_) return;

    sceneParam_->SetRotationZ(value);
    TransformScene();
  }

 private:
  std::unique_ptr<FileReader> fileReader_;
  std::unique_ptr<Scene> scene_;
  std::unique_ptr<SceneParameters> sceneParam_;

  // DrawSceneData DrawScene() { return scene_->DrawScene(); }

  void TransformScene() {
    Mat4f move_mat = TransformMatrixBuilder::CreateMoveMatrix(
        sceneParam_->GetLocationX(), sceneParam_->GetLocationY(),
        sceneParam_->GetLocationZ());
    Mat4f rotate_mat = TransformMatrixBuilder::CreateRotationMatrix(
        sceneParam_->GetRotationX(), sceneParam_->GetRotationY(),
        sceneParam_->GetRotationZ());
    Mat4f scale_mat = TransformMatrixBuilder::CreateScaleMatrix(
        sceneParam_->GetScaleX(), sceneParam_->GetScaleX(),
        sceneParam_->GetScaleX());
    Mat4f transform_mat = scale_mat * move_mat * rotate_mat;
    scene_->TransformSceneMeshData(transform_mat);
  }

};
}  // namespace s21

