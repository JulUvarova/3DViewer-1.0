#pragma once

#include <QDockWidget>
#include <QFileDialog>
#include <QGroupBox>
#include <QImage>
#include <QLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QSlider>
#include <QStatusBar>
#include <QString>
#include <QTimer>
#include <QToolBar>  //!
#include <QWidget>

#include "../controller/controller.h"
#include "BackgroundBox.h"
#include "ElemBox.h"
#include "ProjectionButton.h"
#include "SlidersBox.h"
#include "UserSetting.h"
#include "Viewport3D.h"
#include "gif/gif.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(s21::Controller *controller, QWidget *parent = nullptr);

 public slots:
  void slotLocationCoordX(int coordX);
  void slotLocationCoordY(int coordY);
  void slotLocationCoordZ(int coordZ);

  void slotRotateCoordX(int coordX);
  void slotRotateCoordY(int coordY);
  void slotRotateCoordZ(int coordZ);

  void slotScaleCoordX(int coordX);
  void slotScaleCoordY(int coordY);
  void slotScaleCoordZ(int coordZ);

  void slotVerticesType(const QString &text);
  void slotVerticesSize(const int value);
  void slotVerticesColor(const QColor &color);

  void slotEdgesType(const QString &text);
  void slotEdgesSize(const int value);
  void slotEdgesColor(const QColor &color);

  void slotBackgroundColor(const QColor &color);

  void slotProjectionType(const bool isParallel);

 private:
  // UI elements
  QDockWidget *toolsDock, *propsDock;
  QLabel *propsFileInfo, *propsObjectsInfo;
  SlidersBox *locationSlidersBox, *rotateSlidersBox, *scaleSlidersBox;
  ElemBox *verticesBox, *edgesBox;
  BackgroundBox *backBox;
  QMenuBar *menuBar;
  Viewport3D *renderWindow;
  QPushButton *resetCoordsButton;
  QPushButton *resetElemsButton;
  QPushButton *saveElemsButton;
  QPushButton *restoreElemsButton;

  // Controller
  s21::Controller *controller;

  // user gui settings
  UserSetting *userSetting;

  // for gif
  std::vector<QPixmap> screens;
  QString fileName;
  QTimer *timer;

  void setupUI();
  void createDockWidgets();
  void createMenuAndToolbars();
  void saveLayout();
  void restoreLayout();
  void saveUserSettings();
  void resetUserSettings();
  void restoreUserSettings();
  void openFile();
  void getFileName(const char *options);
  void saveImage();
  void saveCustomGif();
  void saveCycledGif();
  void appExit();
  void closeEvent(QCloseEvent *event) override;
  void setVisualParameters();
  void resetCoords();
  void fillToolsDockWidget();
  void fillPropsDockWidget();
  void grabScene();
};