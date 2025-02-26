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
#include <QToolBar>  //!
#include <QWidget>

#include "../controller/controller.h"
#include "BackgroundBox.h"
#include "ControlWindow.h"
#include "ElemBox.h"
#include "SlidersBox.h"
#include "UserSetting.h"
#include "Viewport3D.h"
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
  QDockWidget *toolsDock, *propsDock;
  QLabel *propsFileInfo, *propsObjectsInfo, *warningInfo;
  SlidersBox *locationSlidersBox, *rotateSlidersBox, *scaleSlidersBox;
  ElemBox *verticesBox, *edgesBox;
  BackgroundBox *backBox;
  QMenuBar *menuBar;
  ControlWindow *controlWindow;
  Viewport3D *renderWindow;
  QPushButton *resetCoordsButton, *resetElemsButton, *saveElemsButton,
      *restoreElemsButton, *sceneInfoButton;
  QRadioButton *centralProj, *perspectiveProj;

  // Controller
  std::shared_ptr<s21::Controller> controller;

  // user gui settings
  std::shared_ptr<UserSetting> userSetting;

  // for saving
  std::vector<QPixmap> screens;
  QString fileName;
  QTimer *timer;

  void setupUI();
  void createDockWidgets();
  void saveLayout();
  void restoreLayout();
  void saveUserSettings();
  void resetUserSettings();
  void restoreUserSettings();
  void loadScene(const char *filename);
  void saveImage(const char *filename);
  void saveCustomGif(const char *filename);
  void saveCycledGif(const char *filename);
  void appExit();
  void closeEvent(QCloseEvent *event) override;
  void setVisualParameters();
  void resetCoords();
  void createStatusBar();
  void grabScene();
  void createGifFile();
};