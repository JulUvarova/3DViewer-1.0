#ifndef FACADE_H
#define FACADE_H

#include "filereader.h"
#include "scene.h"
#include "scene_parameters.h"

namespace s21 {

class Facade {
 public:
  Facade() {
    fileReader_ = new FileReader();
    sceneParam_ = new SceneParameters();
  }

  ~Facade() {
    delete fileReader_;
    delete scene_;
    delete sceneParam_;
  }

  DrawSceneData LoadScene(const char *path) {
    if (scene_) delete scene_;

    scene_ = new Scene();
    scene_->LoadSceneMeshData(fileReader_->ReadFile(path));

    // TODO проверка загрузилась ли сцена
    return scene_->DrawScene();
  }

  DrawSceneData resetScenePosition() {
    if (!scene_) return DrawSceneData();

    sceneParam_ = std::move(new SceneParameters());
    TransformScene();
    return scene_->DrawScene();
  }

  DrawSceneData Scale(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetScaleX(value);
    TransformScene();
    return scene_->DrawScene();
  }

  DrawSceneData ScaleX(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetScaleX(value);
    TransformScene();
    return scene_->DrawScene();
  }

  DrawSceneData ScaleY(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetScaleX(value);
    TransformScene();
    return scene_->DrawScene();
  }

  DrawSceneData ScaleZ(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetScaleX(value);
    TransformScene();
    return scene_->DrawScene();
  }

  DrawSceneData MoveX(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetLocationX(value);
    TransformScene();
    return scene_->DrawScene();
  }

  DrawSceneData MoveY(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetLocationY(value);
    TransformScene();
    return scene_->DrawScene();
  }

  DrawSceneData MoveZ(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetLocationZ(value);
    TransformScene();
    return scene_->DrawScene();
  }

  DrawSceneData RotateX(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetRotationX(value);
    TransformScene();
    return scene_->DrawScene();
  }

  DrawSceneData RotateY(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetRotationY(value);
    TransformScene();
    return scene_->DrawScene();
  }

  DrawSceneData RotateZ(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetRotationZ(value);
    TransformScene();
    return scene_->DrawScene();
  }

 private:
  FileReader *fileReader_;
  Scene *scene_{nullptr};
  SceneParameters *sceneParam_;

  DrawSceneData DrawScene() { return scene_->DrawScene(); }

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

  // void MoveScene(float x, float y, float z) {
  //   Mat4f mat = TransformMatrixBuilder::CreateMoveMatrix(x, y, z);
  //   scene_->TransformSceneMeshData(mat);
  // }

  // void RotateScene(float x, float y, float z) {
  //   Mat4f mat = TransformMatrixBuilder::CreateRotationMatrix(x, y, z);
  //   scene_->TransformSceneMeshData(mat);
  // }

  // void ScaleScene(float x, float y, float z) {
  //   Mat4f mat = TransformMatrixBuilder::CreateScaleMatrix(x, y, z);
  //   scene_->TransformSceneMeshData(mat);
  // }
};
}  // namespace s21
#endif  // FACADE_H
