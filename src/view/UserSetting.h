#pragma once

#include <QColor>
#include <QSettings>
#include <QString>

class UserSetting {
 private:
  const QString fileMemory{"view/settings/usersettings.xml"};

  QString verticesType;
  QColor verticesColor;
  int verticesSize;

  QString edgesType;
  QColor edgesColor;
  int edgesSize;

  QColor backgroundColor;

  bool isParallelProjection;

  QByteArray layoutState;

 public:
  UserSetting() {
    readLayoutSettings();
  }

  ~UserSetting() = default;

  void saveRenderSettings() {
    QSettings settings(fileMemory, QSettings::NativeFormat);

    settings.beginGroup("render");

    settings.setValue("verticesType", verticesType);
    settings.setValue("verticesColor", verticesColor);
    settings.setValue("verticesSize", verticesSize);

    settings.setValue("edgesType", edgesType);
    settings.setValue("edgesColor", edgesColor);
    settings.setValue("edgesSize", edgesSize);

    settings.setValue("backgroundColor", backgroundColor);

    settings.setValue("isParallelProjection", isParallelProjection);

    settings.endGroup();
  }

  void readRenderSettings() {
    QSettings settings(fileMemory, QSettings::NativeFormat);

    settings.beginGroup("render");

    verticesType = settings.value("verticesType", "circle").toString();
    verticesColor =
        settings.value("verticesColor", QColor(Qt::white)).value<QColor>();
    verticesSize = settings.value("verticesSize", 10).toInt();

    edgesType = settings.value("edgesType", "solid").toString();
    edgesColor =
        settings.value("edgesColor", QColor(Qt::white)).value<QColor>();
    edgesSize = settings.value("edgesSize", 10).toInt();

    backgroundColor =
        settings.value("backgroundColor", QColor(Qt::black)).value<QColor>();

    isParallelProjection =
        settings.value("isParallelProjection", true).toBool();

    settings.endGroup();
  }

  void removeRenderSettings() {
    QSettings settings(fileMemory, QSettings::NativeFormat);
    settings.remove("render");
  }

   void saveLayoutSettings() {
    QSettings settings(fileMemory, QSettings::NativeFormat);

    settings.beginGroup("layout");
    settings.setValue("layoutState", layoutState);
    settings.endGroup();
  }

  void readLayoutSettings() {
    QSettings settings(fileMemory, QSettings::NativeFormat);

    settings.beginGroup("layout");
    layoutState = settings.value("layoutState", "").toByteArray();
    settings.endGroup();
  }

  void removeLayoutSettings() {
    QSettings settings(fileMemory, QSettings::NativeFormat);
    settings.remove("layout");
  }

  inline QByteArray getLayoutState() const { return layoutState; }

  inline void setLayoutState(QByteArray state) {
    this->layoutState = state;
  }

  inline bool IsParallelProjectrion() const { return isParallelProjection; }

  inline void setProjection(bool isParallel) {
    this->isParallelProjection = isParallel;
  }

  inline QColor getBackgroundColor() const { return backgroundColor; }

  inline void setBackgroundColor(const QColor &backgroundColor) {
    this->backgroundColor = backgroundColor;
  }

  inline QString getVerticesType() const { return verticesType; }

  inline void setVerticesType(const QString &verticesType) {
    this->verticesType = verticesType;
  }

  inline QColor getVerticesColor() const { return verticesColor; }

  inline void setVerticesColor(const QColor &verticesColor) {
    this->verticesColor = verticesColor;
  }

  inline int getVerticesSize() const { return verticesSize; }

  inline void setVerticesSize(int verticesSize) {
    this->verticesSize = verticesSize;
  }

  inline QString getEdgesType() const { return edgesType; }

  inline void setEdgesType(const QString &edgesType) {
    this->edgesType = edgesType;
  }

  inline QColor getEdgesColor() const { return edgesColor; }

  inline void setEdgesColor(const QColor &edgesColor) {
    this->edgesColor = edgesColor;
  }

  inline int getEdgesSize() const { return edgesSize; }

  inline void setEdgesSize(int edgesSize) { this->edgesSize = edgesSize; }
};