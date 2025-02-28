#pragma once

#include <QColor>
#include <QSettings>
#include <QString>

class UserSetting {
 public:
  UserSetting();

  void SaveRenderSettings();

  void ReadRenderSettings();

  void RemoveRenderSettings();

  inline bool IsParallelProjectrion() const { return isParallelProjection_; }

  inline void SetProjection(bool isParallel) {
    isParallelProjection_ = isParallel;
  }

  inline QColor GetBackgroundColor() const { return backgroundColor_; }

  inline void SetBackgroundColor(const QColor &backgroundColor) {
    backgroundColor_ = backgroundColor;
  }

  inline QString GetVerticesType() const { return verticesType_; }

  inline void SetVerticesType(const QString &verticesType) {
    verticesType_ = verticesType;
  }

  inline QColor GetVerticesColor() const { return verticesColor_; }

  inline void SetVerticesColor(const QColor &verticesColor) {
    verticesColor_ = verticesColor;
  }

  inline int GetVerticesSize() const { return verticesSize_; }

  inline void SetVerticesSize(int verticesSize) {
    verticesSize_ = verticesSize;
  }

  inline QString GetEdgesType() const { return edgesType_; }

  inline void SetEdgesType(const QString &edgesType) { edgesType_ = edgesType; }

  inline QColor GetEdgesColor() const { return edgesColor_; }

  inline void SetEdgesColor(const QColor &edgesColor) {
    edgesColor_ = edgesColor;
  }

  inline int GetEdgesSize() const { return edgesSize_; }

  inline void SetEdgesSize(int edgesSize) { edgesSize_ = edgesSize; }

 private:
  const QString fileMemory_{"view/settings/usersettings.xml"};

  QString verticesType_;
  QColor verticesColor_;
  int verticesSize_;

  QString edgesType_;
  QColor edgesColor_;
  int edgesSize_;

  QColor backgroundColor_;

  bool isParallelProjection_;
};