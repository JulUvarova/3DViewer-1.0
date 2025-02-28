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

#include "control_window.h"
#include "elem_box.h"
#include "info_window.h"
#include "sliders_box.h"
#include "user_setting.h"
#include "viewport3D.h"
#include "background_box.h"
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
  MainWindow(std::shared_ptr<s21::Controller> controller_,
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

 protected:
  void resizeEvent(QResizeEvent *event) override;
  void moveEvent(QMoveEvent *event) override;
  void closeEvent(QCloseEvent *event) override;

 private:
  // UI elements
  QDockWidget *toolsDock_;
  QLabel *filenameInfo_, *warningInfo_;
  SlidersBox *locationSlidersBox_, *rotateSlidersBox_, *scaleSlidersBox_;
  ElemBox *verticesBox_, *edgesBox_;
  BackgroundBox *backgroundBox_;
  QMenuBar *menuBar_;
  ControlWindow *controlWindow_;
  Viewport3D *renderWindow_;
  QPushButton *ResetCoordsButton_, *resetElemsButton_, *saveElemsButton_,
      *restoreElemsButton_, *sceneInfoButton_;
  QRadioButton *perspectiveProj_, *parallelProj_;
  InfoWindow *sceneInfoWindow_;

  // Controller
  std::shared_ptr<s21::Controller> controller_;

  // user gui settings
  std::shared_ptr<UserSetting> userSetting_;

  // for saving
  std::vector<QPixmap> screens_;
  QString filename_;
  QTimer *timer_;

  void SetupUI();
  void CreateDockWidgets();
  void SaveUserSettings();
  void ResetUserSettings();
  void RestoreUserSettings();
  void LoadScene(QString &fname);
  void SaveImage(QString &fname);
  void SaveCustomGif(QString &fname);
  void SaveCycledGif(QString &fname);
  void SetVisualParameters();
  void ResetCoords();
  void CreateStatusBar();
  void GrabScene();
  void CreateGifFile();
  void MoveInfoWindow();
};