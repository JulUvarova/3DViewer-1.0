#pragma once

#include <QGroupBox>
#include <QIntValidator>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QSlider>
#include <QString>
#include <QWidget>
#include <array>

/**
 * @class Slider
 * @brief A custom slider class that emits a signal on double-click.
 *
 * This class extends QSlider to emit a signal when the slider is
 * double-clicked.
 */
class Slider : public QSlider {
  Q_OBJECT

 public:
  /**
   * @brief Constructor for the Slider class.
   *
   * @param parent Pointer to the parent widget (default is nullptr).
   */
  Slider(QWidget *parent = nullptr);

 protected:
  /**
   * @brief Handles the mouse double-click event.
   *
   * @param event The mouse event.
   */
  void mouseDoubleClickEvent([[maybe_unused]] QMouseEvent *event) override {
    Q_EMIT doubleClicked(this);
  }

 Q_SIGNALS:
  /**
   * @brief Signal emitted when the slider is double-clicked.
   *
   * @param slider Pointer to the slider that was double-clicked.
   */
  void doubleClicked(Slider *slider);
};

/**
 * @class SlidersBox
 * @brief A widget containing multiple sliders for coordinate manipulation.
 *
 * This class provides a user interface for adjusting X, Y, and Z coordinates
 * using sliders and corresponding line edits for input.
 */
class SlidersBox : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Constructor for the SlidersBox class.
   *
   * @param name The name of the group box.
   * @param minMax A pair representing the minimum and maximum values for the
   * sliders.
   * @param sliderCount The number of sliders to create.
   * @param parent Pointer to the parent widget (default is nullptr).
   */
  SlidersBox(const char *name, std::pair<int, int> minMax, int sliderCount,
             QWidget *parent = nullptr);

  /**
   * @brief Resets the sliders to their middle position.
   */
  void ResetCoords();

  /**
   * @brief Sets the coordinates of the sliders based on the provided values.
   *
   * @param coordsXY A pair of integers representing the changes in X and Y
   * coordinates.
   */
  void SetCoords(std::pair<int, int> coordsXY);

  /**
   * @brief Gets the current values of the sliders as an array.
   *
   * @return An array containing the current values of the sliders.
   */
  std::array<int, 3> GetCoords() const;

 Q_SIGNALS:
  /**
   * @brief Signal emitted when the X coordinate changes.
   *
   * @param coordX The new value of the X coordinate.
   */
  void signalChangeX(int coordX);

  /**
   * @brief Signal emitted when the Y coordinate changes.
   *
   * @param coordY The new value of the Y coordinate.
   */
  void signalChangeY(int coordY);

  /**
   * @brief Signal emitted when the Z coordinate changes.
   *
   * @param coordZ The new value of the Z coordinate.
   */
  void signalChangeZ(int coordZ);

 private:
  int kSlidersCount;    ///< Number of sliders
  int kMiddle;          ///< Middle value for the sliders
  const int kStep = 1;  ///< Step value for the sliders

  std::array<QLabel *, 3> labels{
      new QLabel{" X ", this}, new QLabel{" Y ", this},
      new QLabel{" Z ", this}};  ///< Labels for the sliders
  std::array<Slider *, 3> sliders{
      new Slider(this), new Slider(this),
      new Slider(this)};  ///< Sliders for coordinate adjustment
  std::array<QLineEdit *, 3> values{
      new QLineEdit(this), new QLineEdit(this),
      new QLineEdit(this)};              ///< Line edits for slider values
  std::array<QHBoxLayout *, 3> layouts;  ///< Layouts for each slider

  /**
   * @brief Resets a specific slider to its middle value.
   *
   * @param slider Pointer to the slider to reset.
   */
  inline void ResetSlider(Slider *slider) { slider->setValue(kMiddle); }

  /**
   * @brief Updates the label and emits a signal for the X slider.
   */
  void LabelChangeX();

  /**
   * @brief Updates the label and emits a signal for the Y slider.
   */
  void LabelChangeY();

  /**
   * @brief Updates the label and emits a signal for the Z slider.
   */
  void LabelChangeZ();

  /**
   * @brief Updates the value and emits a signal for the X slider.
   */
  void SliderChangeX();

  /**
   * @brief Updates the value and emits a signal for the Y slider.
   */
  void SliderChangeY();

  /**
   * @brief Updates the value and emits a signal for the Z slider.
   */
  void SliderChangeZ();
};