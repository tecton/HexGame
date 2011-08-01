#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class QTimer;
class QPixmap;
class AbstractPixmapWidget;
class SwapClassicGame;

class MainWidget : public QWidget
{
  Q_OBJECT
public:
  explicit MainWidget(QWidget *parent = 0);

//  void paint(QPainter *painter);
  void paintEvent(QPaintEvent *event);

protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

private:
  QTimer *refreshTimer;
  QPixmap pixmap;
  QVector<AbstractPixmapWidget *> widgets;

  QPointF toScene(QPointF mousePosition);
signals:

public slots:
  void changeControl(AbstractPixmapWidget *target, bool deleteMySelf);
};

#endif // MAINWIDGET_H
