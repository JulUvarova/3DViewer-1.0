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

  void slotViewportSize(const int w, const int h);

  void slotBackgroundColor(const QColor &color);

  void slotProjectionType(const bool isParallel);

 private:
  QLabel *xValue, *yValue, *zValue;
  QLabel *propsInfo;
  SlidersBox *locationSlidersBox, *rotateSlidersBox, *scaleSlidersBox;
  ElemBox *verticesBox, *edgesBox;
  BackgroundBox *backBox;

  s21::SceneMeshData *scene{nullptr};

  // Controller
  s21::Controller *controller;

  // user choice for vertices & edges
  UserSetting *userSetting;
  Viewport3D *centralWidget;

  // UI elements
  // QDockWidget *toolsDock, *propsDock, *timelineDock;

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
  void setSceneParameters();
  void setVisualParameters();
  void drawScene(s21::DrawSceneData scene);
};