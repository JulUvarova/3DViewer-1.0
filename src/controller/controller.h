#pragma once

#include <iostream>

#include "../model/obj/obj_data.h"
#include "../model/scene.h"
#include "../model/scene_parameters.h"

namespace s21 {

class Controller {
 private:
  SceneParameters* sceneParam_;
  Scene* scene_{nullptr};

 public:
  Controller() : sceneParam_(new SceneParameters()) {}
  ~Controller() { delete sceneParam_; }

  // Upload scene
  SceneMeshData* UploadScene(const char* filename) {
    OBJData raw_obj_data;
    //! получаем OBJData, если FALSE -> "Не удалось загрузить объект" во фронт
    raw_obj_data.Parse(filename);
    raw_obj_data.Normalize();
    //! возвращаем код ER/SUC
    if (!scene_) delete scene_;
    scene_ = new Scene(raw_obj_data);
    return scene_->GetSceneMeshData();
  }

  // Setters
  void SetViewportSize(int w, int h) {
    sceneParam_->SetAspectRate(w / static_cast<float>(h));

    std::cout << "Contr AspectRate: " << sceneParam_->GetAspectRate()
              << std::endl;
  }

  void SetScaleX(const int value) {
    sceneParam_->SetScaleX(value / 100.0);
    std::cout << "Contr Scale X: " << sceneParam_->GetScaleX() << std::endl;
  }

  void SetScaleY(const int value) {
    sceneParam_->SetScaleY(value / 100.0);
    std::cout << "Contr Scale Y: " << sceneParam_->GetScaleY() << std::endl;
  }

  void SetScaleZ(const int value) {
    sceneParam_->SetScaleZ(value / 100.0);
    std::cout << "Contr Scale Z: " << sceneParam_->GetScaleZ() << std::endl;
  }

  void SetLocationX(const int value) {
    sceneParam_->SetLocationX(value / 200.0);
    std::cout << "Contr Location X: " << sceneParam_->GetLocationX()
              << std::endl;
  }

  void SetLocationY(const int value) {
    sceneParam_->SetLocationY(value / 200.0);
    std::cout << "Contr Location Y: " << sceneParam_->GetLocationY()
              << std::endl;
  }

  void SetLocationZ(const int value) {
    sceneParam_->SetLocationZ(value / 200.0);
    std::cout << "Contr Location Z: " << sceneParam_->GetLocationZ()
              << std::endl;
  }

  void SetRotationX(const int value) {
    sceneParam_->SetRotationX(value);
    std::cout << "Contr Rotation X: " << sceneParam_->GetRotationX()
              << std::endl;
  }

  void SetRotationY(const int value) {
    sceneParam_->SetRotationY(value);
    std::cout << "Contr Rotation Y: " << sceneParam_->GetRotationY()
              << std::endl;
  }

  void SetRotationZ(const int value) {
    sceneParam_->SetRotationZ(value);
    std::cout << "Contr Rotation Z: " << sceneParam_->GetRotationZ()
              << std::endl;
  }

  void SetProjectionType(const bool isParallel) {
    if (isParallel)
      sceneParam_->SetProjectionType(ProjectionTypes::kParallel);
    else
      sceneParam_->SetProjectionType(ProjectionTypes::kCentral);
    std::cout << "Contr Projection: change" << std::endl;
  }

  void SetVertexType(const QString type) {
    if (type == "circle") {
      sceneParam_->SetVertexStyle(VertexStyle::kCircle);
    } else if (type == "square") {
      sceneParam_->SetVertexStyle(VertexStyle::kSquare);
    } else {
      sceneParam_->SetVertexStyle(VertexStyle::kNone);
    }
    std::cout << "Contr VertexType: change" << std::endl;
  }

  void SetVertexSize(const int size) {
    sceneParam_->SetVertexSize(size);
    std::cout << "Contr VertexSize: " << sceneParam_->GetVertexSize()
              << std::endl;
  }

  void SetEdgeType(const QString type) {
    if (type == "line") {
      sceneParam_->SetEdgeStyle(EdgeStyle::kLine);
    } else if (type == "dashes") {
      sceneParam_->SetEdgeStyle(EdgeStyle::kDashed);
    } else {
      sceneParam_->SetEdgeStyle(EdgeStyle::kNone);
    }
    std::cout << "Contr EdgeType: change" << std::endl;
  }

  void SetEdgeSize(const int size) {
    sceneParam_->SetEdgeSize(size);
    std::cout << "Contr EdgeSize: " << sceneParam_->GetEdgeSize() << std::endl;
  }

  void SetVertexColor(const QColor& color) {
    sceneParam_->SetVertexColor({static_cast<float>(color.red() / 255.0),
                                 static_cast<float>(color.green() / 255.0),
                                 static_cast<float>(color.blue() / 255.0)});
    std::cout << "Contr VertexColor: r" << sceneParam_->GetVertexColor().r
              << " g" << sceneParam_->GetVertexColor().g << " b"
              << sceneParam_->GetVertexColor().b << std::endl;
  }

  void SetEdgeColor(const QColor& color) {
    sceneParam_->SetEdgeColor({static_cast<float>(color.red() / 255.0),
                               static_cast<float>(color.green() / 255.0),
                               static_cast<float>(color.blue() / 255.0)});
    std::cout << "Contr EdgeColor: r" << sceneParam_->GetEdgeColor().r << " g"
              << sceneParam_->GetEdgeColor().g << " b"
              << sceneParam_->GetEdgeColor().b << std::endl;
  }

  void SetBackgroundColor(const QColor& color) {
    sceneParam_->SetBackgroundColor({static_cast<float>(color.red() / 255.0),
                                     static_cast<float>(color.green() / 255.0),
                                     static_cast<float>(color.blue() / 255.0)});
    std::cout << "Contr BackgroundColor: r"
              << sceneParam_->GetBackgroundColor().r << " g"
              << sceneParam_->GetBackgroundColor().g << " b"
              << sceneParam_->GetBackgroundColor().b << std::endl;
  }
};
}  // namespace s21
