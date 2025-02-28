#pragma once

#include <QLabel>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class InfoWindow : public QWidget {
  Q_OBJECT

 public:
  explicit InfoWindow(QWidget* parent = nullptr);

  void SetText(const QString& text);

  inline void Show() {
    if (!txt_->text().isEmpty()) QWidget::show();
  }

 protected:
  void resizeEvent(QResizeEvent* event) override;

 private:
  QLabel* txt_;
  QPushButton* closeButton_;
};