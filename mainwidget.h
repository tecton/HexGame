#ifndef MAINWIDGET_H
#define MAINWIDGET_H

// File must include
#include <QWidget>
#include <QVector>

// Forward declaration
class QTimer;
class QPixmap;
class AbstractPixmapWidget;
class SwapClassicGame;

class MainWidget : public QWidget
{
  Q_OBJECT
public:
  // Constructor
  explicit MainWidget(QWidget *parent = 0);

  // The paint event(may be called with a fixed interval)
  void paintEvent(QPaintEvent *event);

protected:
  // 3 kinds of mouse event
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

private:
  // The timer to repaint
  QTimer *refreshTimer;

  // The current pixmap
  QPixmap pixmap;

  // The pixmap of the widget just lost control
  QPixmap lastPixmap;

  // The stack of the widgets
  QVector<AbstractPixmapWidget *> widgets;

  // The CD used when change the control
  int coolDown;

  // A function to calculate the position
  QPointF toScene(QPointF mousePosition);
signals:

public slots:
  // A slot to change the control of the widgets in the stack
  void changeControl(AbstractPixmapWidget *target, bool deleteMySelf);
};

#endif // MAINWIDGET_H
