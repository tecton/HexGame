#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMouseEvent;
class QPainter;
class QPaintEvent;
class QTimer;
class AbstractRule;
class AbstractGameBoardInfo;
class CoreController;
class GestureController;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(AbstractRule *theRule,
                      AbstractGameBoardInfo *theGameBoardInfo,
                      QWidget *parent = 0);
  ~MainWindow();

  void paint(QPainter *painter);
  void paintEvent(QPaintEvent *event);


protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

private:
  Ui::MainWindow *ui;
  AbstractRule *rule;
  AbstractGameBoardInfo *gameboardInfo;
  CoreController *controller;
  GestureController *gestureController;
  QTimer *t;
  QPixmap p;

  QPointF toScene(QPointF mousePosition);

public slots:
  void advance();
};

#endif // MAINWINDOW_H
