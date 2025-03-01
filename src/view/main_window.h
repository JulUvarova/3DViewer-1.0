#pragma once

#include <QDockWidget>
#include <QGroupBox>
#include <QImage>
#include <QLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QRadioButton>
#include <QSettings>
#include <QStatusBar>
#include <QString>
#include <QTimer>
#include <QWidget>

#include "background_box.h"
#include "control_window.h"
#include "controller.h"
#include "elem_box.h"
#include "gif/gif.h"
#include "info_window.h"
#include "sliders_box.h"
#include "user_setting.h"
#include "viewport3D.h"

/**
 * @enum TransformType
 * @brief Enum representing different types of transformations.
 */
enum class TransformType {
  LocationX,  ///< Transformation along the X-axis
  LocationY,  ///< Transformation along the Y-axis
  LocationZ,  ///< Transformation along the Z-axis
  RotationX,  ///< Rotation around the X-axis
  RotationY,  ///< Rotation around the Y-axis
  RotationZ,  ///< Rotation around the Z-axis
  Scale       ///< Scaling transformation
};

/**
 * @class MainWindow
 * @brief The main application window for the 3D viewer.
 *
 * This class provides the main interface for the 3D viewer application,
 * including tools for transforming objects, managing settings, and rendering
 * the 3D scene.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Constructor for the MainWindow class.
   *
   * @param controller_ A shared pointer to the application controller.
   * @param parent Pointer to the parent widget (default is nullptr).
   */
  MainWindow(std::shared_ptr<s21::Controller> controller_,
             QWidget *parent = nullptr);

 public Q_SLOTS:
  /**
   * @brief Slot to handle transformations.
   *
   * @param type The type of transformation.
   * @param value The value for the transformation.
   */
  void slotTransform(TransformType type, int value);

  /**
   * @brief Slot to handle changes in vertices type.
   *
   * @param text The new type of vertices.
   */
  void slotVerticesType(const QString &text);

  /**
   * @brief Slot to handle changes in vertices size.
   *
   * @param value The new size of vertices.
   */
  void slotVerticesSize(const int value);

  /**
   * @brief Slot to handle changes in vertices color.
   *
   * @param color The new color of vertices.
   */
  void slotVerticesColor(const QColor &color);

  /**
   * @brief Slot to handle changes in edges type.
   *
   * @param text The new type of edges.
   */
  void slotEdgesType(const QString &text);

  /**
   * @brief Slot to handle changes in edges size.
   *
   * @param value The new size of edges.
   */
  void slotEdgesSize(const int value);

  /**
   * @brief Slot to handle changes in edges color.
   *
   * @param color The new color of edges.
   */
  void slotEdgesColor(const QColor &color);

  /**
   * @brief Slot to handle changes in background color.
   *
   * @param color The new background color.
   */
  void slotBackgroundColor(const QColor &color);

 protected:
  /**
   * @brief Handles the resize event for the main window.
   *
   * @param event The resize event.
   */
  void resizeEvent(QResizeEvent *event) override;

  /**
   * @brief Handles the move event for the main window.
   *
   * @param event The move event.
   */
  void moveEvent(QMoveEvent *event) override;

  /**
   * @brief Handles the close event for the main window.
   *
   * @param event The close event.
   */
  void closeEvent(QCloseEvent *event) override;

 private:
  // UI elements
  QDockWidget *toolsDock_;  ///< Dock widget for tools
  QLabel *filenameInfo_;    ///< Labels for displaying information
  SlidersBox *locationSlidersBox_, *rotateSlidersBox_,
      *scaleSlidersBox_;              ///< Sliders for transformations
  ElemBox *verticesBox_, *edgesBox_;  ///< Boxes for vertices and edges settings
  BackgroundBox *backgroundBox_;      ///< Box for background color settings
  QMenuBar *menuBar_;                 ///< Menu bar for the application
  ControlWindow *controlWindow_;      ///< Control window for file operations
  Viewport3D *renderWindow_;          ///< 3D rendering window
  QPushButton *resetCoordsButton_, *resetElemsButton_, *saveElemsButton_,
      *restoreElemsButton_, *sceneInfoButton_;  ///< Buttons for various actions
  QRadioButton *perspectiveProj_,
      *parallelProj_;  ///< Radio buttons for projection type
  InfoWindow
      *sceneInfoWindow_;  ///< Info window for displaying scene information

  // Controller
  std::shared_ptr<s21::Controller>
      controller_;  ///< Controller for managing application logic

  // User GUI settings
  std::shared_ptr<UserSetting>
      userSetting_;  ///< User settings for the application

  // For saving
  std::vector<QPixmap>
      screens_;       ///< Vector to hold screenshots for GIF creation
  QString filename_;  ///< Filename for saving images or GIFs
  QTimer *timer_;     ///< Timer for GIF animation

  /**
   * @brief Sets up the user interface components.
   */
  void SetupUI();

  /**
   * @brief Creates the dock widgets for the application.
   */
  void CreateDockWidgets();

  /**
   * @brief Saves user settings to persistent storage.
   */
  void SaveUserSettings();

  /**
   * @brief Resets user settings to default values.
   */
  void ResetUserSettings();

  /**
   * @brief Restores user settings from persistent storage.
   */
  void RestoreUserSettings();

  /**
   * @brief Loads a scene from a specified file.
   *
   * @param fname The name of the file to load the scene from.
   */
  void LoadScene(QString &fname);

  /**
   * @brief Saves the current viewport as an image.
   *
   * @param fname The name of the file to save the image to.
   */
  void SaveImage(QString &fname);

  /**
   * @brief Saves a custom GIF based on the current scene.
   *
   * @param fname The name of the file to save the GIF to.
   */
  void SaveCustomGif(QString &fname);

  /**
   * @brief Saves a cycled GIF based on the current scene.
   *
   * @param fname The name of the file to save the GIF to.
   */
  void SaveCycledGif(QString &fname);

  /**
   * @brief Sets visual parameters based on user settings.
   */
  void SetVisualParameters();

  /**
   * @brief Resets the coordinates of the scene.
   */
  void ResetCoords();

  /**
   * @brief Creates the status bar for the application.
   */
  void CreateStatusBar();

  /**
   * @brief Captures the current scene for GIF creation.
   */
  void GrabScene();

  /**
   * @brief Creates a GIF file from captured frames.
   */
  void CreateGifFile();

  /**
   * @brief Moves the info window based on the main window's position.
   */
  void MoveInfoWindow();
};