#pragma once

#include <QLabel>
#include <QLayout>
#include <QOpenGLFunctions>
#include <QSlider>
#include <QString>
#include <QWidget>

struct Coords {
  int x, y, z;
};

class SlidersBox : public QWidget {
  Q_OBJECT

  int step = 1;
  QLabel *xLabel, *yLabel, *zLabel;
  QSlider *xSlider, *ySlider, *zSlider;
  QLabel *xValue, *yValue, *zValue;
  QHBoxLayout *xLayout, *yLayout, *zLayout;
  QVBoxLayout *layout;

  inline void xChange() {
    xValue->setText(QString::number(xSlider->value()));
    emit signalChangeX(xSlider->value());
  }
  inline void yChange() {
    yValue->setText(QString::number(ySlider->value()));
    emit signalChangeY(ySlider->value());
  }
  inline void zChange() {
    zValue->setText(QString::number(zSlider->value()));
    emit signalChangeZ(zSlider->value());
  }

 signals:
  void signalChangeX(int coordX);
  void signalChangeY(int coordY);
  void signalChangeZ(int coordZ);

 public:
  SlidersBox(const char *name, float min, float max, QWidget *parent = nullptr)
      : QWidget(parent) {
    xLabel = new QLabel(" X ");
    yLabel = new QLabel(" Y ");
    zLabel = new QLabel(" Z ");

    int midle = (min + max) / 2;
    xSlider = new QSlider(Qt::Horizontal, this);
    xSlider->setRange(min, max);
    xSlider->setTickInterval(step);
    xSlider->setValue(midle);
    //! doesn't work?
    // xSlider->setStyleSheet("QSlider::add-page:horizontal { background: green;
    // }");

    ySlider = new QSlider(Qt::Horizontal, this);
    ySlider->setRange(min, max);
    ySlider->setTickInterval(step);
    ySlider->setValue(midle);

    zSlider = new QSlider(Qt::Horizontal, this);
    zSlider->setRange(min, max);
    zSlider->setTickInterval(step);
    zSlider->setValue(midle);

    xValue = new QLabel(QString::number(xSlider->value()));
    xValue->setFixedWidth(4 * QFontMetrics(xValue->font()).averageCharWidth());
    xValue->setAlignment(Qt::AlignRight);
    yValue = new QLabel(QString::number(ySlider->value()));
    yValue->setFixedWidth(4 * QFontMetrics(yValue->font()).averageCharWidth());
    yValue->setAlignment(Qt::AlignRight);
    zValue = new QLabel(QString::number(zSlider->value()));
    zValue->setFixedWidth(4 * QFontMetrics(zValue->font()).averageCharWidth());
    zValue->setAlignment(Qt::AlignRight);

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

    connect(xSlider, &QSlider::valueChanged, this, &SlidersBox::xChange);
    connect(ySlider, &QSlider::valueChanged, this, &SlidersBox::yChange);
    connect(zSlider, &QSlider::valueChanged, this, &SlidersBox::zChange);
  };

 private:
  Coords sendCoords() {
    return Coords{xSlider->value(), ySlider->value(), zSlider->value()};
  }
};