#include "sliders_box.h"

Slider::Slider(QWidget *parent) : QSlider(parent) {
  setOrientation(Qt::Horizontal);
  setStyleSheet(
      "QSlider { background-color: rgba(240, 236, 236, 0); } "
      "QSlider::groove:horizontal { background-color: grey; "
      "height: 4px; border: 1px solid black;} "
      "QSlider::handle:horizontal { "
      "background: lightgrey; width: 7px; height: 12px; "
      "margin: -7px 0; border-radius: 2px; border: 1px solid black; }");
}

SlidersBox::SlidersBox(const char *name, std::pair<int, int> minMax,
                       int sliderCount, QWidget *parent)
    : QWidget(parent), kSlidersCount(sliderCount) {
  QVBoxLayout *layout = new QVBoxLayout();
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

    connect(sliders[i], &Slider::doubleClicked, this, &SlidersBox::ResetSlider);
  }

  QGroupBox *groupBox = new QGroupBox(name);
  groupBox->setLayout(layout);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(groupBox);
  setLayout(mainLayout);

  connect(sliders[0], &QSlider::valueChanged, this, &SlidersBox::SliderChangeX);
  connect(sliders[1], &QSlider::valueChanged, this, &SlidersBox::SliderChangeY);
  connect(sliders[2], &QSlider::valueChanged, this, &SlidersBox::SliderChangeZ);

  connect(values[0], &QLineEdit::textChanged, this, &SlidersBox::LabelChangeX);
  connect(values[1], &QLineEdit::textChanged, this, &SlidersBox::LabelChangeY);
  connect(values[2], &QLineEdit::textChanged, this, &SlidersBox::LabelChangeZ);
};

void SlidersBox::ResetCoords() {
  for (int i = 0; i < kSlidersCount; ++i) sliders[i]->setValue(kMiddle);
}

void SlidersBox::SetCoords(std::pair<int, int> coordsXY) {
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

std::array<int, 3> SlidersBox::GetCoords() const {
  std::array<int, 3> coords = {sliders[0]->value(),
                               (kSlidersCount > 1) ? sliders[1]->value() : 0,
                               (kSlidersCount > 2) ? sliders[2]->value() : 0};
  return coords;
}

void SlidersBox::LabelChangeX() {
  sliders[0]->setValue(values[0]->text().toInt());
  Q_EMIT signalChangeX(sliders[0]->value());
}

void SlidersBox::LabelChangeY() {
  sliders[1]->setValue(values[1]->text().toInt());
  Q_EMIT signalChangeY(sliders[1]->value());
}

void SlidersBox::LabelChangeZ() {
  sliders[2]->setValue(values[2]->text().toInt());
  Q_EMIT signalChangeZ(sliders[2]->value());
}

void SlidersBox::SliderChangeX() {
  values[0]->setText(QString::number(sliders[0]->value()));
  Q_EMIT signalChangeX(sliders[0]->value());
}

void SlidersBox::SliderChangeY() {
  values[1]->setText(QString::number(sliders[1]->value()));
  Q_EMIT signalChangeY(sliders[1]->value());
}

void SlidersBox::SliderChangeZ() {
  values[2]->setText(QString::number(sliders[2]->value()));
  Q_EMIT signalChangeZ(sliders[2]->value());
}