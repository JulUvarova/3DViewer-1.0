#pragma once

#include <QColor>
#include <QColorDialog>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QOpenGLFunctions>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QWidget>

struct Setting {
  QString type;
  QColor color;
  int size;
};

class ElemBox : public QWidget {
  Q_OBJECT

 public:
  ElemBox(const char *name, QStringList &lst, Setting &setting,
          QWidget *parent = nullptr);

  void SetSetting(Setting &setting);

 Q_SIGNALS:
  void signalChangeType(const QString &text);
  void signalChangeSize(const int size);
  void signalChangeColor(const QColor &color);

 private:
  QLabel *typeLabel_, *sizeLabel_, *colorLabel_;
  QSpinBox *size_;
  QColor color_;
  QPushButton *colorButton_;
  QComboBox *type_;

  inline void TypeChange(const QString &text) { Q_EMIT signalChangeType(text); }
  inline void SizeChange(const int value) { Q_EMIT signalChangeSize(value); }
  void ColorChange();
  void SetColorButton();
};