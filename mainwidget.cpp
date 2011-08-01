#include "mainwidget.h"

#include <QApplication>
#include <QTimer>
#include <QPixmap>
#include <QPointF>
#include <QPainter>
#include <QPaintEvent>
#include "abstractpixmapwidget.h"
#include "mainmenu.h"

#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
  MainMenu *mainMenu = new MainMenu();
  widgets.push_back(mainMenu);
  connect(mainMenu,
          SIGNAL(giveControlTo(AbstractPixmapWidget*,bool)),
          this,
          SLOT(changeControl(AbstractPixmapWidget*,bool)));
  refreshTimer = new QTimer();
  refreshTimer->setInterval(30);
  connect(refreshTimer, SIGNAL(timeout()),this, SLOT(update()));
  refreshTimer->start();
//  QMessageBox::critical(0,"","MainWidget created");
}

void MainWidget::paintEvent(QPaintEvent *event)
{
  widgets[widgets.size() - 1]->makePixmap(pixmap, width(), height());
//  QMessageBox::critical(0,"","MainWidget pixmap made");
  QPainter *painter = new QPainter(this);
  painter->drawPixmap(0,0/*,width(), height()*/, pixmap);
//  paint(painter);
  painter->end();
  delete painter;
//  QMessageBox::critical(0,"","MainWidget painted");
}


QPointF MainWidget::toScene(QPointF mousePosition)
{
  double xRate = mousePosition.x() / width();
  double yRate = mousePosition.y()/ height();
  return widgets[widgets.size() - 1]->toScene(xRate, yRate);
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
  QPointF pos = toScene(event->posF());
  widgets[widgets.size() - 1]->dealPressed(pos, event->button());
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
  QPointF pos = toScene(event->posF());
  widgets[widgets.size() - 1]->dealMoved(pos, event->button());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
  QPointF pos = toScene(event->posF());
  widgets[widgets.size() - 1]->dealReleased(pos, event->button());
}

void MainWidget::changeControl(AbstractPixmapWidget *target,
                               bool deleteMySelf)
{
  if (deleteMySelf)
    widgets.pop_back();
  if (target)
  {
    widgets.push_back(target);
    connect(target,
            SIGNAL(giveControlTo(AbstractPixmapWidget*,bool)),
            this,
            SLOT(changeControl(AbstractPixmapWidget*,bool)));
  }
  if (widgets.isEmpty())
  {
    qApp->exit();
    //delete this; //something wrong with it
  }
}
