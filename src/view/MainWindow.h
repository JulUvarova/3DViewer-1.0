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
#include <QWidget>

#include "../model/obj/obj_data.h"
#include "CollapseButton.h"
#include "ElemBox.h"
#include "RenderSetting.h"
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
  void slotVerticesType(const QString &text);
  void slotVerticesSize(const int value);
  void slotVerticesColor(const QColor &color);
  void slotEdgesType(const QString &text);
  void slotEdgesSize(const int value);
  void slotEdgesColor(const QColor &color);
  void slotViewportSize(const int w, const int h);

 private:
  //! useless
  QSlider *xSlider, *ySlider, *zSlider;
  QLabel *xValue, *yValue, *zValue;
  QLabel *propsInfo;
  SlidersBox *moveSlidersBox, *rotateSlidersBox, *scaleSlidersBox;
  ElemBox *verticesBox, *edgesBox;
  //!

  // Check parser
  s21::OBJData obj_data;

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
};