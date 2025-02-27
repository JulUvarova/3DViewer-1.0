#include <QLabel>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class InfoWindow : public QWidget {
 public:
  explicit InfoWindow(QWidget* parent = nullptr) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool |
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet(
        "background-color: rgba(50, 50, 50, 200); color: white; border-radius: "
        "5px;");

    QVBoxLayout* layout = new QVBoxLayout(this);

    txt = new QLabel();
    txt->setWordWrap(true);
    layout->addWidget(txt);

    closeButton = new QPushButton("×", this);
    closeButton->setStyleSheet("border: none; font-size: 16px;");
    closeButton->setFixedSize(20, 20);
    connect(closeButton, &QPushButton::clicked, this, &QWidget::hide);

    closeButton->move(width() - closeButton->width() - 5, 5);
  }

  void setText(const QString& text) {
    txt->setText(text);
    txt->adjustSize();
    adjustSize();
    resize(size() * 1.5);
    txt->setAlignment(Qt::AlignCenter);
  }

  void show() {
    if (!txt->text().isEmpty()) QWidget::show();
  }

 protected:
  void resizeEvent(QResizeEvent* event) override {
    closeButton->move(width() - closeButton->width(), 0);
    QWidget::resizeEvent(event);
  }

 private:
  QLabel* txt;
  QPushButton* closeButton;
};