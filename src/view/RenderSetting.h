#pragma once

#include <QColor>
#include <QSettings>
#include <QString>

class RenderSetting {
 private:
  QString verticesType;
  QColor verticesColor;
  int verticesSize;

  QString edgesType;
  QColor edgesColor;
  int edgesSize;

 public:
  RenderSetting() { readRenderSetting(); }
  ~RenderSetting() = default;

  void saveRenderSetting() {
    QSettings settings("./usersettings.xml", QSettings::NativeFormat);

    settings.setValue("verticesType", verticesType);
    settings.setValue("verticesColor", verticesColor);
    settings.setValue("verticesSize", verticesSize);

    settings.setValue("edgesType", edgesType);
    settings.setValue("edgesColor", edgesColor);
    settings.setValue("edgesSize", edgesSize);
  }

  void readRenderSetting() {
    QSettings settings("./usersettings.xml", QSettings::NativeFormat);

    verticesType = settings.value("verticesType", "circle").toString();
    verticesColor =
        settings.value("verticesColor", QColor(Qt::white)).value<QColor>();
    verticesSize = settings.value("verticesSize", 10).toInt();

    edgesType = settings.value("edgesType", "solid").toString();
    edgesColor =
        settings.value("edgesColor", QColor(Qt::white)).value<QColor>();
    edgesSize = settings.value("edgesSize", 10).toInt();
  }

  inline QString getVerticesType() const { return verticesType; }

  inline  void setVerticesType(const QString &verticesType) {
    this->verticesType = verticesType;
  }

  inline QColor getVerticesColor() const { return verticesColor; }

inline   void setVerticesColor(const QColor &verticesColor) {
    this->verticesColor = verticesColor;
  }

  inline int getVerticesSize() const { return verticesSize; }

  inline void setVerticesSize(int verticesSize) { this->verticesSize = verticesSize; }

  inline QString getEdgesType() const { return edgesType; }

  inline void setEdgesType(const QString &edgesType) { this->edgesType = edgesType; }

  inline QColor getEdgesColor() const { return edgesColor; }

  inline void setEdgesColor(const QColor &edgesColor) {
    this->edgesColor = edgesColor;
  }

  inline int getEdgesSize() const { return edgesSize; }

  inline void setEdgesSize(int edgesSize) { this->edgesSize = edgesSize; }
};