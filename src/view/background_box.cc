#include "background_box.h"

BackgroundBox::BackgroundBox(const char *name, QColor clr, QWidget *parent)
    : QWidget(parent) {
  // color
  colorLabel_ = new QLabel("Color:", this);
  colorButton_ = new QPushButton(" ", this);
  color_ = clr;
  SetColorButton();

  // boxing
  QHBoxLayout *colorLayout = new QHBoxLayout();
  colorLayout->addWidget(colorLabel_);
  colorLayout->addWidget(colorButton_);

  QVBoxLayout *groupBoxLayout = new QVBoxLayout();
  groupBoxLayout->addLayout(colorLayout);

  QGroupBox *groupBox = new QGroupBox(name, this);
  groupBox->setLayout(groupBoxLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(groupBox);
  setLayout(mainLayout);

  connect(colorButton_, &QPushButton::clicked, this,
          &BackgroundBox::ColorChange);
};

void BackgroundBox::ColorChange() {
  QColorDialog *colorDialog = new QColorDialog(this);
  colorDialog->setCurrentColor(color_);
  if (colorDialog->exec() == QDialog::Accepted) {
    color_ = colorDialog->currentColor();
    SetColorButton();
    Q_EMIT signalChangeColor(color_);
  }
}

void BackgroundBox::SetColorButton() {
  colorButton_->setStyleSheet("border: none; background-color: rgb(" +
                              QString::number(color_.red()) + ", " +
                              QString::number(color_.green()) + ", " +
                              QString::number(color_.blue()) + ")");
}

void BackgroundBox::SetSetting(QColor clr) {
  color_ = clr;
  SetColorButton();
}