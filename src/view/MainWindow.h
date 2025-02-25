#pragma once

#include <QByteArray>
#include <QDockWidget>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QSlider>
#include <QStatusBar>
#include <QString>
#include <QToolBar>
#include <QWidget>

#include "../controller/controller.h"
#include "BackgroundBox.h"
#include "ElemBox.h"
#include "ProjectionButton.h"
#include "SlidersBox.h"
#include "UserSetting.h"
#include "Viewport3D.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(s21::Controller *controller, QWidget *parent = nullptr);

 public Q_SLOTS:
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

  // scene data
  // s21::DrawSceneData *scene{nullptr};

  void setupUI();
  void createDockWidgets();
  void createMenuAndToolbars();
  void saveLayout();
  void restoreLayout();
  void saveUserSettings();
  void resetUserSettings();
  void restoreUserSettings();
  void openFile();
  void saveImage();
  void appExit();
  void closeEvent(QCloseEvent *event) override;
  void setVisualParameters();
  // void drawScene(s21::DrawSceneData scene);
  void resetCoords();
  void fillToolsDockWidget();
  void fillPropsDockWidget();
};