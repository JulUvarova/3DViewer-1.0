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
#include "../model/obj/obj_data.h"
#include "BackgroundBox.h"
#include "ElemBox.h"
#include "RenderSetting.h"
#include "SlidersBox.h"
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
  //! useless
  QSlider *xSlider, *ySlider, *zSlider;
  QLabel *xValue, *yValue, *zValue;
  QLabel *propsInfo;
  SlidersBox *locationSlidersBox, *rotateSlidersBox, *scaleSlidersBox;
  ElemBox *verticesBox, *edgesBox;
  BackgroundBox *backBox;
  //!

  // Check parser
  s21::OBJData obj_data;

  // Controller
  s21::Controller *controller;

  // user choice for vertices & edges
  RenderSetting *renderSetting;
  Viewport3D *centralWidget;

  void setupUI();
  void createDockWidgets();
  void createMenuAndToolbars();
  void saveLayout();
  void restoreLayout();
  void openFile();
  void saveImage();
  void appExit();
  void closeEvent(QCloseEvent *event) override;
  void setAllParameters();
};