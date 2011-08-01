#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QTimer>

#include "abstractrule.h"
#include "abstractgameboardinfo.h"
#include "corecontroller.h"
#include "gesturecontroller.h"

#include "ball.h"
#include "connections.h"
#include "basicpainter.h"


MainWindow::MainWindow(AbstractRule *theRule,
                       AbstractGameBoardInfo *theGameBoardInfo,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    rule(theRule),
    gameboardInfo(theGameBoardInfo)
{
  ui->setupUi(this);
  controller = new CoreController(theRule, theGameBoardInfo);
  controller->fillAllBlanks();
  gestureController = new GestureController(rule, gameboardInfo, controller);
  p = QPixmap(width(), height());

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();
}

MainWindow::~MainWindow()
{
  delete ui;
  delete controller;
  delete gameboardInfo;
  delete rule;
  delete gestureController;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
  p = QPixmap(width(), height());
  p.fill(Qt::black);
  QPainter *painter = new QPainter(&p);
  paint(painter);
  painter->end();
  delete painter;


  /*QPainter **/painter = new QPainter(this);
  painter->drawPixmap(0,0/*,width(), height()*/, p);
//  paint(painter);
  painter->end();
  delete painter;
}

void MainWindow::paint(QPainter *painter)
{
  Ball **balls = controller->balls;
  BasicPainter::paintBasicBalls(balls,
                                gameboardInfo->totalBallCounts(),
                                painter,
                                width() * 1.0 / gameboardInfo->width(),
                                height() * 1.0 / gameboardInfo->height(),
                                0);

}

QPointF MainWindow::toScene(QPointF mousePosition)
{
  double xRate = mousePosition.x() / width();
  double yRate = mousePosition.y()/ height();
  return QPointF(xRate * gameboardInfo->width(),
                 yRate * gameboardInfo->height());
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
  QPointF pos = toScene(event->posF());
  gestureController->dealPressed(pos);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
  QPointF pos = toScene(event->posF());
  gestureController->dealMoved(pos);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
  QPointF pos = toScene(event->posF());
  gestureController->dealReleased(pos);
}

void MainWindow::advance()
{
  controller->advance();
  update();
}
