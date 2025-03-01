#pragma once

#include <QColor>
#include <QColorDialog>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QWidget>

/**
 * @struct Setting
 * @brief Structure to hold settings for an element.
 *
 * This structure contains the type, color, and size of an element.
 */
struct Setting {
  QString type;  ///< The type of the element.
  QColor color;  ///< The color of the element.
  int size;      ///< The size of the element.
};

/**
 * @class ElemBox
 * @brief A class for configuring an element's settings.
 *
 * This class provides a user interface for selecting the type, size, and color
 * of an element. It emits signals when these settings change.
 */
class ElemBox : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Constructor for the ElemBox class.
   *
   * @param name The name of the group box.
   * @param lst A list of types to populate the combo box.
   * @param setting The initial settings for the element.
   * @param parent Pointer to the parent widget (default is nullptr).
   */
  ElemBox(const char *name, QStringList &lst, Setting &setting,
          QWidget *parent = nullptr);

  /**
   * @brief Sets the settings for the element.
   *
   * @param setting The new settings to apply.
   */
  void SetSetting(Setting &setting);

 Q_SIGNALS:
  /**
   * @brief Signal emitted when the type changes.
   *
   * @param text The new type of the element.
   */
  void signalChangeType(const QString &text);

  /**
   * @brief Signal emitted when the size changes.
   *
   * @param size The new size of the element.
   */
  void signalChangeSize(const int size);

  /**
   * @brief Signal emitted when the color changes.
   *
   * @param color The new color of the element.
   */
  void signalChangeColor(const QColor &color);

 private:
  QLabel *typeLabel_;         ///< Label for the type selection.
  QLabel *sizeLabel_;         ///< Label for the size selection.
  QLabel *colorLabel_;        ///< Label for the color selection.
  QSpinBox *size_;            ///< Spin box for selecting the size.
  QPushButton *colorButton_;  ///< Button to display and select the color.
  QComboBox *type_;           ///< Combo box for selecting the type.

  /**
   * @brief Emits the signalChangeType signal with the new type.
   *
   * @param text The new type of the element.
   */
  inline void TypeChange(const QString &text) { Q_EMIT signalChangeType(text); }

  /**
   * @brief Emits the signalChangeSize signal with the new size of the element.
   *
   * @param value The new size of the element.
   */
  inline void SizeChange(const int value) { Q_EMIT signalChangeSize(value); }

  /**
   * @brief Opens a color dialog to change the color.
   */
  void ColorChange();

  /**
   * @brief Updates the color button to reflect the current color.
   * @param color The new color of the element.
   */
  void SetColorButton(const QColor &color);
};