#pragma once

#include <QDockWidget>
#include <QFileDialog>
#include <QGroupBox>
#include <QImage>
#include <QLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QRadioButton>
#include <QSettings>
#include <QSlider>
#include <QStatusBar>
#include <QString>
#include <QTimer>
#include <QWidget>

#include "BackgroundBox.h"
#include "ControlWindow.h"
#include "ElemBox.h"
#include "InfoWindow.h"
#include "SlidersBox.h"
#include "UserSetting.h"
#include "Viewport3D.h"
#include "controller.h"
#include "gif/gif.h"

enum class TransformType {
  LocationX,
  LocationY,
  LocationZ,
  RotationX,
  RotationY,
  RotationZ,
  Scale
};

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(std::shared_ptr<s21::Controller> controller,
             QWidget *parent = nullptr);

 public Q_SLOTS:
  void slotTransform(TransformType type, int value);

  void slotVerticesType(const QString &text);
  void slotVerticesSize(const int value);
  void slotVerticesColor(const QColor &color);

  void slotEdgesType(const QString &text);
  void slotEdgesSize(const int value);
  void slotEdgesColor(const QColor &color);

  void slotBackgroundColor(const QColor &color);

 private:
  // UI elements
  QDockWidget *toolsDock;
  QLabel *propsFileInfo, *warningInfo;
  SlidersBox *locationSlidersBox, *rotateSlidersBox, *scaleSlidersBox;
  ElemBox *verticesBox, *edgesBox;
  BackgroundBox *backBox;
  QMenuBar *menuBar;
  ControlWindow *controlWindow;
  Viewport3D *renderWindow;
  QPushButton *resetCoordsButton, *resetElemsButton, *saveElemsButton,
      *restoreElemsButton, *sceneInfoButton;
  QRadioButton *perspectiveProj, *parallelProj;
  InfoWindow *sceneInfoWindow;

  // Controller
  std::shared_ptr<s21::Controller> controller;

  // user gui settings
  std::shared_ptr<UserSetting> userSetting;

  // for saving
  std::vector<QPixmap> screens;
  QString filename;
  QTimer *timer;

  void setupUI();
  void createDockWidgets();
  void saveUserSettings();
  void resetUserSettings();
  void restoreUserSettings();
  void loadScene(QString& fname);
  void saveImage(QString& fname);
  void saveCustomGif(QString& fname);
  void saveCycledGif(QString& fname);
  void closeEvent(QCloseEvent *event) override;
  void setVisualParameters();
  void resetCoords();
  void createStatusBar();
  void grabScene();
  void createGifFile();
  void moveInfoWindow();

 protected:
  void resizeEvent(QResizeEvent *event) override;
  void moveEvent(QMoveEvent *event) override;
};