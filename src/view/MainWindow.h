#pragma once

#include <QByteArray>
#include <QDockWidget>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QSettings>
#include <QSlider>
#include <QStatusBar>
#include <QString>
#include <QToolBar>

#include "../model/obj/obj_data.h"
#include "SlidersBox.h"
#include "Viewport3D.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);

 public slots:
  void slotMoveCoords(Coords coords);
  void slotScaleCoords(Coords coords);
  void slotRotateCoords(Coords coords);

 private:
  QSlider *xSlider, *ySlider, *zSlider;
  QLabel *xValue, *yValue, *zValue;
  QLabel *propsInfo;
  SlidersBox *moveSlidersBox, *rotateSlidersBox, *scaleSlidersBox;

  // Check parser
  s21::OBJData obj_data;

  void setupUI();
  void createDockWidgets();
  void createMenuAndToolbars();
  void saveLayout();
  void restoreLayout();
  void openFile();
  void saveImage();
  void close();
};