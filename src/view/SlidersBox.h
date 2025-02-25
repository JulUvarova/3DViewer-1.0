#pragma once

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
  Slider(QWidget *parent = nullptr) : QSlider(parent) {
    setOrientation(Qt::Horizontal);
    setStyleSheet(
        "QSlider { background-color: rgba(240, 236, 236, 0); } "
        "QSlider::groove:horizontal { background-color: grey; "
        "height: 4px; border: 1px solid black;} "
        "QSlider::handle:horizontal { "
        "background: lightgrey; width: 7px; height: 12px; "
        "margin: -7px 0; border-radius: 2px; border: 1px solid black; }");
  }

 protected:
  void mouseDoubleClickEvent([[maybe_unused]] QMouseEvent *event) override {
    Q_EMIT doubleClicked(this);
  }

 Q_SIGNALS:
  void doubleClicked(Slider *slider);
};

class SlidersBox : public QWidget {
  Q_OBJECT

  int kSlidersCount;
  int kMiddle;
  const int kStep = 1;

  Slider *xSlider, *ySlider, *zSlider;
  QLineEdit *xValue, *yValue, *zValue;
  QHBoxLayout *xLayout, *yLayout, *zLayout;
  QVBoxLayout *layout;

  std::array<QLabel *, 3> labels{new QLabel{" X "}, new QLabel{" Y "},
                                 new QLabel{" Z "}};
  std::array<Slider *, 3> sliders{new Slider(), new Slider(), new Slider()};
  std::array<QLineEdit *, 3> values{new QLineEdit(), new QLineEdit(),
                                    new QLineEdit()};
  std::array<QHBoxLayout *, 3> layouts{new QHBoxLayout(), new QHBoxLayout(),
                                       new QHBoxLayout()};

 Q_SIGNALS:
  void signalChangeX(int coordX);
  void signalChangeY(int coordY);
  void signalChangeZ(int coordZ);

 public:
  SlidersBox(const char *name, std::pair<int, int> minMax, int sliderCount,
             QWidget *parent = nullptr)
      : QWidget(parent), kSlidersCount(sliderCount) {
    layout = new QVBoxLayout();
    kMiddle = (minMax.first + minMax.second) / 2;
    QIntValidator *validator =
        new QIntValidator(minMax.first, minMax.second, this);

    for (int i = 0; i < kSlidersCount; ++i) {
      sliders[i] = new Slider(this);
      sliders[i]->setRange(minMax.first, minMax.second);
      sliders[i]->setTickInterval(kStep);
      sliders[i]->setValue(kMiddle);

      values[i] = new QLineEdit(this);
      values[i]->setValidator(validator);
      values[i]->setText(QString::number(sliders[i]->value()));
      values[i]->setFixedWidth(
          4 * QFontMetrics(values[i]->font()).averageCharWidth());
      values[i]->setAlignment(Qt::AlignRight);
      values[i]->setFrame(false);

      layouts[i] = new QHBoxLayout();
      layouts[i]->addWidget(labels[i]);
      layouts[i]->addWidget(sliders[i]);
      layouts[i]->addWidget(values[i]);

      layout->addLayout(layouts[i]);

      connect(sliders[i], &Slider::doubleClicked, this,
              &SlidersBox::resetSlider);
    }

    QGroupBox *groupBox = new QGroupBox(name);
    groupBox->setLayout(layout);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(groupBox);
    setLayout(mainLayout);

    connect(sliders[0], &QSlider::valueChanged, this,
            &SlidersBox::sliderChangeX);
    connect(sliders[1], &QSlider::valueChanged, this,
            &SlidersBox::sliderChangeY);
    connect(sliders[2], &QSlider::valueChanged, this,
            &SlidersBox::sliderChangeZ);

    connect(values[0], &QLineEdit::textChanged, this,
            &SlidersBox::labelChangeX);
    connect(values[1], &QLineEdit::textChanged, this,
            &SlidersBox::labelChangeY);
    connect(values[2], &QLineEdit::textChanged, this,
            &SlidersBox::labelChangeZ);
  };

  void resetCoords() {
    for (int i = 0; i < kSlidersCount; ++i) sliders[i]->setValue(kMiddle);
  }

   void setCoords(std::pair<int, int> coordsXY) {
    if (coordsXY.first) {
      sliders[0]->setValue(sliders[0]->value() + coordsXY.first);
      values[0]->setText(QString::number(sliders[0]->value()));
      Q_EMIT signalChangeX(sliders[0]->value());
    }
    if (coordsXY.second) {
      sliders[1]->setValue(sliders[1]->value() + coordsXY.second);
      values[1]->setText(QString::number(sliders[1]->value()));
      Q_EMIT signalChangeY(sliders[1]->value());
    }
  }

  std::array<int, 3> getCoords() {
    std::array<int, 3> coords = {sliders[0]->value(),
                                 (kSlidersCount > 1) ? sliders[1]->value() : 0,
                                 (kSlidersCount > 2) ? sliders[2]->value() : 0};
    return coords;
  }

 private:
  void resetSlider(Slider *slider) {
    slider->setValue(kMiddle);
  }

  void labelChangeX() {
    sliders[0]->setValue(values[0]->text().toInt());
    Q_EMIT signalChangeX(sliders[0]->value());
  }

  void labelChangeY() {
    sliders[1]->setValue(values[1]->text().toInt());
    Q_EMIT signalChangeY(sliders[1]->value());
  }

  void labelChangeZ() {
    sliders[2]->setValue(values[2]->text().toInt());
    Q_EMIT signalChangeZ(sliders[2]->value());
  }

  void sliderChangeX() {
    values[0]->setText(QString::number(sliders[0]->value()));
    Q_EMIT signalChangeX(sliders[0]->value());
  }

  void sliderChangeY() {
    values[1]->setText(QString::number(sliders[1]->value()));
    Q_EMIT signalChangeY(sliders[1]->value());
  }

  void sliderChangeZ() {
    values[2]->setText(QString::number(sliders[2]->value()));
    Q_EMIT signalChangeZ(sliders[2]->value());
  }
};