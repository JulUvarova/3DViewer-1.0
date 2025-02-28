#pragma once

#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QIcon>
#include <QMenu>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QPushButton>
#include <QWidget>

enum class MouseAction {
  kNone = 0,
  kLeftButton,
  kMiddleButton,
};

class ControlWindow : public QWidget {
  Q_OBJECT

 public:
  ControlWindow(QWidget* parent = nullptr);

 Q_SIGNALS:
  void signalChangeSize(const int w, const int h);
  void signalChangeMoveCoords(std::pair<int, int> coordXY);
  void signalChangeScaleCoords(std::pair<int, int> coordXY);
  void signalChangeRotateCoords(std::pair<int, int> coordXY);
  void signalOpenFile(QString& fname);
  void signalSaveFile(QString& fname);
  void signalStartCustomGif(QString& fname);
  void signalStartCycledGif(QString& fname);

 protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent([[maybe_unused]] QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

 private:
  QPushButton *openButton_, *jpegButton_, *bmpButton_, *customGifButton_,
      *cycledGifButton_;
  QMenu* settingMenu_;
  // catch mouse press and mouse release
  MouseAction mouseAction_ = MouseAction::kNone;
  QPoint mousePos_;

  void OpenFile();
  void SaveFile(const char* options, int gif);
};