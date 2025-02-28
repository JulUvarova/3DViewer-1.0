#include "elem_box.h"

ElemBox::ElemBox(const char *name, QStringList &lst, Setting &setting,
                 QWidget *parent)
    : QWidget(parent) {
  // type
  typeLabel_ = new QLabel("Type:");
  type_ = new QComboBox(this);
  type_->addItems(lst);
  type_->setCurrentText(setting.type);
  type_->setEditable(true);
  QHBoxLayout *typeLayout = new QHBoxLayout();
  typeLayout->addWidget(typeLabel_);
  typeLayout->addWidget(type_);

  // size
  sizeLabel_ = new QLabel("Size:");
  size_ = new QSpinBox(this);
  size_->setRange(0, 15);
  size_->setValue(setting.size);
  QHBoxLayout *sizeLayout = new QHBoxLayout();
  sizeLayout->addWidget(sizeLabel_);
  sizeLayout->addWidget(size_);

  // color
  colorLabel_ = new QLabel("Color:");
  colorButton_ = new QPushButton(" ");
  color_ = setting.color;
  SetColorButton();

  // boxing
  QHBoxLayout *colorLayout = new QHBoxLayout();
  colorLayout->addWidget(colorLabel_);
  colorLayout->addWidget(colorButton_);

  QVBoxLayout *groupBoxLayout = new QVBoxLayout();
  groupBoxLayout->addLayout(typeLayout);
  groupBoxLayout->addLayout(sizeLayout);
  groupBoxLayout->addLayout(colorLayout);

  QGroupBox *groupBox = new QGroupBox(name);
  groupBox->setLayout(groupBoxLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(groupBox);
  setLayout(mainLayout);

  connect(type_, &QComboBox::currentTextChanged, this, &ElemBox::TypeChange);
  connect(size_, &QSpinBox::valueChanged, this, &ElemBox::SizeChange);
  connect(colorButton_, &QPushButton::clicked, this, &ElemBox::ColorChange);
};

void ElemBox::SetSetting(Setting &setting) {
  type_->setCurrentText(setting.type);
  size_->setValue(setting.size);
  color_ = setting.color;
  SetColorButton();
}

void ElemBox::ColorChange() {
  QColorDialog *colorDialog = new QColorDialog(this);
  colorDialog->setCurrentColor(color_);
  if (colorDialog->exec() == QDialog::Accepted) {
    color_ = colorDialog->currentColor();
    SetColorButton();
    Q_EMIT signalChangeColor(color_);
  }
}

void ElemBox::SetColorButton() {
  colorButton_->setStyleSheet("border: none; background-color: rgb(" +
                              QString::number(color_.red()) + ", " +
                              QString::number(color_.green()) + ", " +
                              QString::number(color_.blue()) + ")");
}