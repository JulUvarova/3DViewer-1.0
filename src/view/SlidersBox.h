#pragma once

#include <QLabel>
#include <QLayout>
#include <QOpenGLFunctions>
#include <QSlider>
#include <QString>
#include <QWidget>

struct Coords {
  double x, y, z;
};

class SlidersBox : public QWidget {
  Q_OBJECT

  const float min = -100.0;
  const float max = 100.0;
  const float midle = (min + max) / 2.0;
  const float step = 1;
  QLabel *xLabel, *yLabel, *zLabel;
  QSlider *xSlider, *ySlider, *zSlider;
  QLabel *xValue, *yValue, *zValue;
  QHBoxLayout *xLayout, *yLayout, *zLayout;
  QVBoxLayout *layout;

  inline void xChange() {
    xValue->setText(QString::number(xSlider->value() / 100.0, 'f', 2));
    emit signalChangeCoords(sendCoords());
  }
  inline void yChange() {
    yValue->setText(QString::number(ySlider->value() / 100.0, 'f', 2));
    emit signalChangeCoords(sendCoords());
  }
  inline void zChange() {
    zValue->setText(QString::number(zSlider->value() / 100.0, 'f', 2));
    emit signalChangeCoords(sendCoords());
  }

 signals:
  void signalChangeCoords(Coords coords);

 public:
  SlidersBox(const char *name, QWidget *parent = nullptr) : QWidget(parent) {
    xLabel = new QLabel(" X ");
    yLabel = new QLabel(" Y ");
    zLabel = new QLabel(" Z ");
    xSlider = new QSlider(this);
    xSlider->setRange(min, max);
    xSlider->setTickInterval(step);
    xSlider->setValue(midle);
    xSlider->setOrientation(Qt::Horizontal);

    ySlider = new QSlider(this);
    ySlider->setRange(min, max);
    ySlider->setTickInterval(step);
    ySlider->setValue(midle);
    ySlider->setOrientation(Qt::Horizontal);

    zSlider = new QSlider(this);
    zSlider->setRange(min, max);
    zSlider->setTickInterval(step);
    zSlider->setValue(midle);
    zSlider->setOrientation(Qt::Horizontal);

    xValue = new QLabel(QString::number(xSlider->value()));
    yValue = new QLabel(QString::number(ySlider->value()));
    zValue = new QLabel(QString::number(zSlider->value()));

    xLayout = new QHBoxLayout();
    xLayout->addWidget(xLabel);
    xLayout->addWidget(xSlider);
    xLayout->addWidget(xValue);

    yLayout = new QHBoxLayout();
    yLayout->addWidget(yLabel);
    yLayout->addWidget(ySlider);
    yLayout->addWidget(yValue);

    zLayout = new QHBoxLayout();
    zLayout->addWidget(zLabel);
    zLayout->addWidget(zSlider);
    zLayout->addWidget(zValue);

    layout = new QVBoxLayout();
    layout->addLayout(xLayout);
    layout->addLayout(yLayout);
    layout->addLayout(zLayout);

    QGroupBox *groupBox = new QGroupBox(name);
    groupBox->setLayout(layout);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(groupBox);
    setLayout(mainLayout);

    connect(xSlider, &QSlider::sliderMoved, this, &SlidersBox::xChange);
    connect(ySlider, &QSlider::sliderMoved, this, &SlidersBox::yChange);
    connect(zSlider, &QSlider::sliderMoved, this, &SlidersBox::zChange);
  };

 private:
  Coords sendCoords() {
    return Coords{xSlider->value() / 100.0, ySlider->value() / 100.0,
                  zSlider->value() / 100.0};
  }
};