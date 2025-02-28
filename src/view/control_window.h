#pragma once

#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QIcon>
#include <QMouseEvent>
#include <QPushButton>
#include <QWidget>

/**
 * @enum MouseAction
 * @brief Enum representing mouse actions.
 */
enum class MouseAction {
  kNone = 0, ///< No mouse action
  kLeftButton, ///< Left mouse button action
  kMiddleButton, ///< Middle mouse button action
};

/**
 * @class ControlWindow
 * @brief A class for controlling various actions in the application.
 * 
 * This class provides a user interface for opening and saving files, as well as
 * handling mouse events for moving and rotating objects.
 */
class ControlWindow : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Constructor for the ControlWindow class.
   * 
   * @param parent Pointer to the parent widget (default is nullptr).
   */
  ControlWindow(QWidget* parent = nullptr);

 Q_SIGNALS:
  /**
   * @brief Signal emitted when the window size changes.
   * 
   * @param w New width.
   * @param h New height.
   */
  void signalChangeSize(const int w, const int h);

  /**
   * @brief Signal emitted when the coordinates for moving change.
   * 
   * @param coordXY New coordinates for movement.
   */
  void signalChangeMoveCoords(std::pair<int, int> coordXY);

  /**
   * @brief Signal emitted when the coordinates for scaling change.
   * 
   * @param coordXY New coordinates for scaling.
   */
  void signalChangeScaleCoords(std::pair<int, int> coordXY);

  /**
   * @brief Signal emitted when the coordinates for rotation change.
   * 
   * @param coordXY New coordinates for rotation.
   */
  void signalChangeRotateCoords(std::pair<int, int> coordXY);

  /**
   * @brief Signal emitted to open a file.
   * 
   * @param fname The name of the file to open.
   */
  void signalOpenFile(QString& fname);

  /**
   * @brief Signal emitted to save a file.
   * 
   * @param fname The name of the file to save.
   */
  void signalSaveFile(QString& fname);

  /**
   * @brief Signal emitted to start a custom GIF.
   * 
   * @param fname The name of the GIF file to start.
   */
  void signalStartCustomGif(QString& fname);

  /**
   * @brief Signal emitted to start a cycled GIF.
   * 
   * @param fname The name of the GIF file to start.
   */
  void signalStartCycledGif(QString& fname);

 protected:
  /**
   * @brief Handles mouse press events.
   * 
   * @param event The mouse event.
   */
  void mousePressEvent(QMouseEvent* event) override;

  /**
   * @brief Handles mouse move events.
   * 
   * @param event The mouse event.
   */
  void mouseMoveEvent(QMouseEvent* event) override;

  /**
   * @brief Handles mouse release events.
   * 
   * @param event The mouse event.
   */
  void mouseReleaseEvent([[maybe_unused]] QMouseEvent* event) override;

  /**
   * @brief Handles mouse wheel events.
   * 
   * @param event The wheel event.
   */
  void wheelEvent(QWheelEvent* event) override;

 private:
  QPushButton *openButton_, *jpegButton_, *bmpButton_, *customGifButton_,
      *cycledGifButton_; ///< Buttons for various actions
  MouseAction mouseAction_ = MouseAction::kNone; ///< Current mouse action
  QPoint mousePos_; ///< Current mouse position

  /**
   * @brief Opens a file dialog to select a file.
   */
  void OpenFile();

  /**
   * @brief Saves a file with the specified options.
   * 
   * @param options The file options (e.g., file extension).
   * @param type The type of saving (0 for image, 1 for custom gif, 2 for cycled gif).
   */
  void SaveFile(const char* options, int type); //TODO
};