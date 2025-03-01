#pragma once

#include <QColor>
#include <QColorDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QOpenGLFunctions>
#include <QPushButton>
#include <QString>
#include <QWidget>

/**
 * @class BackgroundBox
 * @brief A class for selecting and displaying a background color.
 *
 * This class provides an interface for selecting a color using a dialog window
 * and displays the selected color as a button. It also signals when the color
 * changes.
 */
class BackgroundBox : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Constructor for the BackgroundBox class.
   *
   * @param name The name of the group displayed in the header.
   * @param clr The initial background color.
   * @param parent Pointer to the parent widget (default is nullptr).
   */
  BackgroundBox(const char *name, QColor clr, QWidget *parent = nullptr);

  /**
   * @brief Sets the button color according to the current color.
   *
   * @param color The new color of the element.
   *
   * Updates the button's style to display the current color.
   */
  void SetColorButton(const QColor &color);

 Q_SIGNALS:
  /**
   * @brief Signal emitted when the color changes.
   *
   * @param color The newly selected color.
   */
  void signalChangeColor(const QColor color);

 private:
  QLabel *colorLabel_;        ///< Label to display the text "Color:"
  QVBoxLayout *layout_;       ///< Main vertical layout for the widget
  QPushButton *colorButton_;  ///< Button to display the selected color

  /**
   * @brief Handles color change.
   *
   * Opens a color selection dialog and updates the color if the user
   * confirms the selection.
   */
  void ColorChange();
};
