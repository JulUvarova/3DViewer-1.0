#pragma once

#include <QLabel>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

/**
 * @class InfoWindow
 * @brief A class for displaying an informational window with scene
 * characteristics.
 *
 * This class provides a simple window to display text messages with a close
 * button. The window is frameless and can be styled to be translucent.
 */
class InfoWindow : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Constructor for the InfoWindow class.
   *
   * @param parent Pointer to the parent widget (default is nullptr).
   */
  explicit InfoWindow(QWidget* parent = nullptr);

  /**
   * @brief Sets the text to be displayed in the info window.
   *
   * @param text The text to display.
   */
  void SetText(const QString& text);

  /**
   * @brief Shows the info window if the text is not empty.
   */
  inline void Show() {
    if (!txt_->text().isEmpty()) QWidget::show();
  }

 protected:
  /**
   * @brief Handles the resize event for the widget.
   *
   * @param event The resize event.
   */
  void resizeEvent(QResizeEvent* event) override;

 private:
  QLabel* txt_;               ///< Label to display the text.
  QPushButton* closeButton_;  ///< Button to close the info window.
};