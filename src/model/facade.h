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

  DrawSceneData Scale(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetScaleX(value);
    TransformScene(sceneParam_->GetLocationX(), sceneParam_->GetLocationY(),
                   sceneParam_->GetLocationZ(), sceneParam_->GetRotationX(),
                   sceneParam_->GetRotationY(), sceneParam_->GetRotationZ(),
                   sceneParam_->GetScaleX());
    // ScaleScene(value, value, value);
    return scene_->DrawScene();
  }

  DrawSceneData ScaleX(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetScaleX(value);
    TransformScene(sceneParam_->GetLocationX(), sceneParam_->GetLocationY(),
                   sceneParam_->GetLocationZ(), sceneParam_->GetRotationX(),
                   sceneParam_->GetRotationY(), sceneParam_->GetRotationZ(),
                   sceneParam_->GetScaleX());
    // ScaleScene(value - sceneParam_->GetScaleX(), 0, 0);
    return scene_->DrawScene();
  }

  DrawSceneData ScaleY(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetScaleX(value);
    TransformScene(sceneParam_->GetLocationX(), sceneParam_->GetLocationY(),
                   sceneParam_->GetLocationZ(), sceneParam_->GetRotationX(),
                   sceneParam_->GetRotationY(), sceneParam_->GetRotationZ(),
                   sceneParam_->GetScaleX());
    // ScaleScene(0, value - sceneParam_->GetScaleY(), 0);
    return scene_->DrawScene();
  }

  DrawSceneData ScaleZ(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetScaleX(value);
    TransformScene(sceneParam_->GetLocationX(), sceneParam_->GetLocationY(),
                   sceneParam_->GetLocationZ(), sceneParam_->GetRotationX(),
                   sceneParam_->GetRotationY(), sceneParam_->GetRotationZ(),
                   sceneParam_->GetScaleX());
    // ScaleScene(0, 0, value - sceneParam_->GetScaleZ());
    return scene_->DrawScene();
  }

  DrawSceneData MoveX(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetLocationX(value);
    TransformScene(sceneParam_->GetLocationX(), sceneParam_->GetLocationY(),
                   sceneParam_->GetLocationZ(), sceneParam_->GetRotationX(),
                   sceneParam_->GetRotationY(), sceneParam_->GetRotationZ(),
                   sceneParam_->GetScaleX());
    // MoveScene(sceneParam_->GetLocationX(), 0, 0);
    return scene_->DrawScene();
  }

  DrawSceneData MoveY(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetLocationY(value);
    TransformScene(sceneParam_->GetLocationX(), sceneParam_->GetLocationY(),
                   sceneParam_->GetLocationZ(), sceneParam_->GetRotationX(),
                   sceneParam_->GetRotationY(), sceneParam_->GetRotationZ(),
                   sceneParam_->GetScaleX());
    // MoveScene(0, sceneParam_->GetLocationY(), 0);
    return scene_->DrawScene();
  }

  DrawSceneData MoveZ(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetLocationZ(value);
    TransformScene(sceneParam_->GetLocationX(), sceneParam_->GetLocationY(),
                   sceneParam_->GetLocationZ(), sceneParam_->GetRotationX(),
                   sceneParam_->GetRotationY(), sceneParam_->GetRotationZ(),
                   sceneParam_->GetScaleX());
    // MoveScene(0, 0, sceneParam_->GetLocationZ());
    return scene_->DrawScene();
  }

  DrawSceneData RotateX(const float value) {
    if (!scene_) return DrawSceneData();

    std::cout << value << std::endl;  //!

    sceneParam_->SetRotationX(value);
    TransformScene(sceneParam_->GetLocationX(), sceneParam_->GetLocationY(),
                   sceneParam_->GetLocationZ(), sceneParam_->GetRotationX(),
                   sceneParam_->GetRotationY(), sceneParam_->GetRotationZ(),
                   sceneParam_->GetScaleX());
    // RotateScene(sceneParam_->GetRotationX(), sceneParam_->GetRotationY(),
    //             sceneParam_->GetRotationZ());
    return scene_->DrawScene();
  }

  DrawSceneData RotateY(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetRotationY(value);
    TransformScene(sceneParam_->GetLocationX(), sceneParam_->GetLocationY(),
                   sceneParam_->GetLocationZ(), sceneParam_->GetRotationX(),
                   sceneParam_->GetRotationY(), sceneParam_->GetRotationZ(),
                   sceneParam_->GetScaleX());
    // RotateScene(sceneParam_->GetRotationX(), sceneParam_->GetRotationY(),
    //             sceneParam_->GetRotationZ());
    return scene_->DrawScene();
  }

  DrawSceneData RotateZ(const float value) {
    if (!scene_) return DrawSceneData();

    sceneParam_->SetRotationZ(value);
    TransformScene(sceneParam_->GetLocationX(), sceneParam_->GetLocationY(),
                   sceneParam_->GetLocationZ(), sceneParam_->GetRotationX(),
                   sceneParam_->GetRotationY(), sceneParam_->GetRotationZ(),
                   sceneParam_->GetScaleX());
    //  RotateScene(sceneParam_->GetRotationX(),
    //                            sceneParam_->GetRotationY(),
    //                            sceneParam_->GetRotationZ());
    return scene_->DrawScene();
  }

 private:
  FileReader *fileReader_;
  Scene *scene_{nullptr};
  SceneParameters *sceneParam_;

  DrawSceneData DrawScene() { return scene_->DrawScene(); }

  void TransformScene(float m_x, float m_y, float m_z, float r_x, float r_y,
                      float r_z, float scale) {
    Mat4f move_mat = TransformMatrixBuilder::CreateMoveMatrix(m_x, m_y, m_z);
    Mat4f rotate_mat =
        TransformMatrixBuilder::CreateRotationMatrix(r_x, r_y, r_z);
    Mat4f scale_mat =
        TransformMatrixBuilder::CreateScaleMatrix(scale, scale, scale);
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
