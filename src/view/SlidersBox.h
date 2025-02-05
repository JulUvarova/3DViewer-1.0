#pragma once

#include <QGroupBox>
#include <QLabel>
#include <QLayout>

class SlidersBox : public QGroupBox {
  int min = 0;
  int max = 100;
  float step = 0.01;
  QLabel xLabel = new QLabel(" X ");
  QLabel yLabel = new QLabel(" Y ");
  QLabel xLabel = new QLabel(" X ");

 public:
  SlidersBox(const char *name) : SlidersBox() {
    xSlider = new QSlider;
    xSlider->setRange(min, max);
    xSlider->setTickInterval(step);
    xSlider->setValue(max / 2);
    // xSlider->setTickPosition(QSlider::NoTicks);
    xSlider->setOrientation(Qt::Horizontal);

    ySlider = new QSlider(xSlider);
    ySlider->setValue(max / 2);
    ySlider->setOrientation(Qt::Horizontal);

    zSlider = new QSlider(xSlider);
    zSlider->setValue(max / 2);
    zSlider->setOrientation(Qt::Horizontal);

    QGroupBox *xInfo = new QGroupBox();
    QHBoxLayout *xLayout = new QHBoxLayout();
    xLayout->addWidget(xLabel);
    xLayout->addWidget(xSlider);
    xInfo->setLayout(xLayout);

    QGroupBox *yInfo = new QGroupBox();
    QHBoxLayout *yLayout = new QHBoxLayout();
    yLayout->addWidget(yLabel);
    yLayout->addWidget(ySlider);
    yInfo->setLayout(yLayout);

    QGroupBox *zInfo = new QGroupBox();
    QHBoxLayout *zLayout = new QHBoxLayout();
    zLayout->addWidget(zLabel);
    zLayout->addWidget(zSlider);
    zInfo->setLayout(zLayout);

    QGroupBox *group = new QGroupBox();
    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(xInfo);
    layout->addWidget(yInfo);
    layout->addWidget(zInfo);
    group->setLayout(layout);

    group->setTitle(name);
  };
}