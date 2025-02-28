#pragma once

#include <QGroupBox>
#include <QIntValidator>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QOpenGLFunctions>
#include <QSlider>
#include <QString>
#include <QWidget>
#include <array>

class Slider : public QSlider {
  Q_OBJECT

 public:
  Slider(QWidget *parent = nullptr);

 protected:
  void mouseDoubleClickEvent([[maybe_unused]] QMouseEvent *event) override {
    Q_EMIT doubleClicked(this);
  }

 Q_SIGNALS:
  void doubleClicked(Slider *slider);
};

class SlidersBox : public QWidget {
  Q_OBJECT

 public:
  SlidersBox(const char *name, std::pair<int, int> minMax, int sliderCount,
             QWidget *parent = nullptr);

  void ResetCoords();

  void SetCoords(std::pair<int, int> coordsXY);
  std::array<int, 3> GetCoords() const;

 Q_SIGNALS:
  void signalChangeX(int coordX);
  void signalChangeY(int coordY);
  void signalChangeZ(int coordZ);

 private:
  int kSlidersCount;
  int kMiddle;
  const int kStep = 1;

  std::array<QLabel *, 3> labels{new QLabel{" X ", this},
                                 new QLabel{" Y ", this},
                                 new QLabel{" Z ", this}};
  std::array<Slider *, 3> sliders{new Slider(this), new Slider(this),
                                  new Slider(this)};
  std::array<QLineEdit *, 3> values{new QLineEdit(this), new QLineEdit(this),
                                    new QLineEdit(this)};
  std::array<QHBoxLayout *, 3> layouts;

  inline void ResetSlider(Slider *slider) { slider->setValue(kMiddle); }

  void LabelChangeX();

  void LabelChangeY();

  void LabelChangeZ();

  void SliderChangeX();

  void SliderChangeY();

  void SliderChangeZ();
};