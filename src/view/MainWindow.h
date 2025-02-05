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

#include "Viewport3D.h"
#include "../model/obj/obj_data.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);

 private slots:
  void saveLayout();
  void restoreLayout();
  void openFile();
  void saveImage();
  void close();

  // отладка
  void xChange();
  void yChange();
  void zChange();

 private:
  QSlider *xSlider, *ySlider, *zSlider;
  QLabel *propsInfo;

  // Check parser
  s21::OBJData obj_data;

  void setupUI();
  void createDockWidgets();
  void createMenuAndToolbars();
};