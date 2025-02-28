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

class BackgroundBox : public QWidget {
  Q_OBJECT

 public:
  BackgroundBox(const char *name, QColor clr, QWidget *parent = nullptr);
  void SetSetting(QColor clr);

 Q_SIGNALS:
  void signalChangeColor(const QColor color);

 private:
  QLabel *colorLabel_;
  QVBoxLayout *layout_;
  QColor color_;
  QPushButton *colorButton_;

  void ColorChange();
  void SetColorButton();
};