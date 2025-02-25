#pragma once

#include "filereader.h"
#include "scene.h"
#include "scene_parameters.h"
#include <functional>

namespace s21 {

class Facade {
 public:
  // Define a callback type for scene updates
  using SceneUpdateCallback = std::function<void(const std::shared_ptr<DrawSceneData>&)>;
  
  Facade() {
    fileReader_ = std::make_unique<FileReader>();
    sceneParam_ = std::make_unique<SceneParameters>();
  }

  ~Facade() {}

  // Set callback for scene updates
  void SetSceneUpdateCallback(SceneUpdateCallback callback) {
    sceneUpdateCallback_ = callback;
  }

  std::shared_ptr<DrawSceneData> LoadScene(const char *path) {
    scene_.reset();
    scene_ = std::make_unique<Scene>();
    auto sceneData = scene_->LoadSceneMeshData(fileReader_->ReadFile(path));
    
    // Store the initial scene data
    if (sceneData) {
      currentSceneData_ = sceneData;
    }
    
    return sceneData;
  }

  void resetScenePosition() {
    if (!scene_) return;

    sceneParam_ = std::make_unique<SceneParameters>();
    TransformScene();
  }

  void Scale(const float value) {
    if (!scene_) return;

    sceneParam_->SetScaleX(value);
    sceneParam_->SetScaleY(value);
    sceneParam_->SetScaleZ(value);
    TransformScene();
  }

  void ScaleX(const float value) {
    if (!scene_) return;

    sceneParam_->SetScaleX(value);
    TransformScene();
  }

  void ScaleY(const float value) {
    if (!scene_) return;

    sceneParam_->SetScaleY(value);
    TransformScene();
  }

  void ScaleZ(const float value) {
    if (!scene_) return;

    sceneParam_->SetScaleZ(value);
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
  std::shared_ptr<DrawSceneData> currentSceneData_;
  SceneUpdateCallback sceneUpdateCallback_;

  // DrawSceneData DrawScene() { return scene_->DrawScene(); }

  void TransformScene() {
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

};
}  // namespace s21

