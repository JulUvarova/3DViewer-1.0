#include "user_setting.h"

UserSetting::UserSetting() { ReadRenderSettings(); }

void UserSetting::SaveRenderSettings() {
  QSettings settings(fileMemory_, QSettings::NativeFormat);

  settings.beginGroup("render");

  settings.setValue("verticesType", verticesType_);
  settings.setValue("verticesColor", verticesColor_);
  settings.setValue("verticesSize", verticesSize_);

  settings.setValue("edgesType", edgesType_);
  settings.setValue("edgesColor", edgesColor_);
  settings.setValue("edgesSize", edgesSize_);

  settings.setValue("backgroundColor", backgroundColor_);

  settings.setValue("isParallelProjection", isParallelProjection_);

  settings.endGroup();
}

void UserSetting::ReadRenderSettings() {
  QSettings settings(fileMemory_, QSettings::NativeFormat);

  settings.beginGroup("render");

  verticesType_ = settings.value("verticesType", "circle").toString();
  verticesColor_ =
      settings.value("verticesColor", QColor(Qt::white)).value<QColor>();
  verticesSize_ = settings.value("verticesSize", 5).toInt();

  edgesType_ = settings.value("edgesType", "solid").toString();
  edgesColor_ = settings.value("edgesColor", QColor(Qt::white)).value<QColor>();
  edgesSize_ = settings.value("edgesSize", 5).toInt();

  backgroundColor_ =
      settings.value("backgroundColor", QColor(Qt::black)).value<QColor>();

  isParallelProjection_ = settings.value("isParallelProjection", true).toBool();

  settings.endGroup();
}

void UserSetting::ResetRenderSettings() {
  verticesType_ = "circle";
  verticesColor_ = QColor(Qt::white);
  verticesSize_ = 5;

  edgesType_ = "solid";
  edgesColor_ = QColor(Qt::white);
  edgesSize_ = 5;

  backgroundColor_ = QColor(Qt::black);

  isParallelProjection_ = true;
}