#pragma once

#include <iostream>

#include "../model/facade.h"
#include "../model/scene.h"
#include "../model/scene_parameters.h"

namespace s21 {

class Controller {
 private:
  Facade* facade_;

  // TODO исправить
  const float kScaleCorrection = 100.0;
  const float kRotationCorrection = 1.0;
  const float kMoveCorrection = 200.0;

 public:
  Controller() { facade_ = new Facade(); }

  ~Controller() { delete facade_; }

  // Upload scene
  DrawSceneData LoadScene(const char* filename) {
    return facade_->LoadScene(filename);
  }

  //! Если изменения всегда по одной координате - 
  //! не разбивать на X Y Z функции 

  // Setters
  DrawSceneData SetScaleX(const int value) {
    return facade_->Scale(value / kScaleCorrection);
  }

  DrawSceneData SetScaleY(const int value) {
    return facade_->Scale(value / kScaleCorrection);
  }

  DrawSceneData SetScaleZ(const int value) {
    return facade_->Scale(value / kScaleCorrection);
  }

  DrawSceneData SetLocationX(const int value) {
    std::cout << value << std::endl;
    return facade_->MoveX(value / kMoveCorrection);
  }

  DrawSceneData SetLocationY(const int value) {
    return facade_->MoveY(value / kMoveCorrection);
  }

  DrawSceneData SetLocationZ(const int value) {
    return facade_->MoveZ(value / kMoveCorrection);
  }

  DrawSceneData SetRotationX(const int value) {
    return facade_->RotateX(value / kRotationCorrection);
  }

  DrawSceneData SetRotationY(const int value) {
    return facade_->RotateY(value / kRotationCorrection);
  }

  DrawSceneData SetRotationZ(const int value) {
    return facade_->RotateZ(value / kRotationCorrection);
  }

  //! Цвета и размеры
  // void SetProjectionType(const bool isParallel) {
  //   if (isParallel)
  //     sceneParam_->SetProjectionType(ProjectionTypes::kParallel);
  //   else
  //     sceneParam_->SetProjectionType(ProjectionTypes::kCentral);
  //   std::cout << "Contr Projection: change" << std::endl;
  // }

  // void SetVertexType(const QString type) {
  //   if (type == "circle") {
  //     sceneParam_->SetVertexStyle(VertexStyle::kCircle);
  //   } else if (type == "square") {
  //     sceneParam_->SetVertexStyle(VertexStyle::kSquare);
  //   } else {
  //     sceneParam_->SetVertexStyle(VertexStyle::kNone);
  //   }
  //   std::cout << "Contr VertexType: change" << std::endl;
  // }

  // void SetVertexSize(const int size) {
  //   sceneParam_->SetVertexSize(size);
  //   std::cout << "Contr VertexSize: " << sceneParam_->GetVertexSize()
  //             << std::endl;
  // }

  // void SetEdgeType(const QString type) {
  //   if (type == "line") {
  //     sceneParam_->SetEdgeStyle(EdgeStyle::kLine);
  //   } else if (type == "dashes") {
  //     sceneParam_->SetEdgeStyle(EdgeStyle::kDashed);
  //   } else {
  //     sceneParam_->SetEdgeStyle(EdgeStyle::kNone);
  //   }
  //   std::cout << "Contr EdgeType: change" << std::endl;
  // }

  // void SetEdgeSize(const int size) {
  //   sceneParam_->SetEdgeSize(size);
  //   std::cout << "Contr EdgeSize: " << sceneParam_->GetEdgeSize() <<
  //   std::endl;
  // }

  // void SetVertexColor(const QColor& color) {
  //   sceneParam_->SetVertexColor({static_cast<float>(color.red() / 255.0),
  //                                static_cast<float>(color.green() / 255.0),
  //                                static_cast<float>(color.blue() / 255.0)});
  //   std::cout << "Contr VertexColor: r" << sceneParam_->GetVertexColor().r
  //             << " g" << sceneParam_->GetVertexColor().g << " b"
  //             << sceneParam_->GetVertexColor().b << std::endl;
  // }

  // void SetEdgeColor(const QColor& color) {
  //   sceneParam_->SetEdgeColor({static_cast<float>(color.red() / 255.0),
  //                              static_cast<float>(color.green() / 255.0),
  //                              static_cast<float>(color.blue() / 255.0)});
  //   std::cout << "Contr EdgeColor: r" << sceneParam_->GetEdgeColor().r << "
  //   g"
  //             << sceneParam_->GetEdgeColor().g << " b"
  //             << sceneParam_->GetEdgeColor().b << std::endl;
  // }

  // void SetBackgroundColor(const QColor& color) {
  //   sceneParam_->SetBackgroundColor({static_cast<float>(color.red() / 255.0),
  //                                    static_cast<float>(color.green() /
  //                                    255.0), static_cast<float>(color.blue()
  //                                    / 255.0)});
  //   std::cout << "Contr BackgroundColor: r"
  //             << sceneParam_->GetBackgroundColor().r << " g"
  //             << sceneParam_->GetBackgroundColor().g << " b"
  //             << sceneParam_->GetBackgroundColor().b << std::endl;
  // }

  // void SetViewportSize(int w, int h) {
  //   //! ?
  //   sceneParam_->SetAspectRate(w / static_cast<float>(h));

  //   std::cout << "Contr AspectRate: " << sceneParam_->GetAspectRate()
  //             << std::endl;
  // }
};
}  // namespace s21
