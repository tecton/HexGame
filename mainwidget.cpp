#include "mainwidget.h"

#include <QApplication>
#include <QTimer>
#include <QPixmap>
#include <QPointF>
#include <QPainter>
#include <QPaintEvent>
#include "abstractpixmapwidget.h"
#include "mainmenuwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    coolDown(0)
{
  MainMenuWidget *mainMenu = new MainMenuWidget();
  widgets.push_back(mainMenu);
  connect(mainMenu,
          SIGNAL(giveControlTo(AbstractPixmapWidget*,bool)),
          this,
          SLOT(changeControl(AbstractPixmapWidget*,bool)));
  refreshTimer = new QTimer();
  refreshTimer->setInterval(30);
  connect(refreshTimer, SIGNAL(timeout()),this, SLOT(update()));
  refreshTimer->start();
}

void MainWidget::paintEvent(QPaintEvent *event)
{
  if (coolDown > 0)
    --coolDown;
  widgets[widgets.size() - 1]->makePixmap(pixmap, width(), height());
  QPainter *painter = new QPainter(this);
  if (coolDown > 0)
  {
    painter->drawPixmap(0,0,width(), height(), lastPixmap);
    painter->drawPixmap(0,-coolDown * height() / 20,width(), height(), pixmap);
  }
  else
    painter->drawPixmap(0,0/*,width(), height()*/, pixmap);
//  paint(painter);
  painter->end();
  delete painter;
}


QPointF MainWidget::toScene(QPointF mousePosition)
{
  double xRate = mousePosition.x() / width();
  double yRate = mousePosition.y()/ height();
  return widgets[widgets.size() - 1]->toScene(xRate, yRate);
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
  if (coolDown > 0)
    return;
  QPointF pos = toScene(event->posF());
  widgets[widgets.size() - 1]->dealPressed(pos, event->button());
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
  if (coolDown > 0)
    return;
  QPointF pos = toScene(event->posF());
  widgets[widgets.size() - 1]->dealMoved(pos, event->button());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (coolDown > 0)
    return;
  QPointF pos = toScene(event->posF());
  widgets[widgets.size() - 1]->dealReleased(pos, event->button());
}

void MainWidget::changeControl(AbstractPixmapWidget *target,
                               bool deleteMySelf)
{
  widgets[widgets.size() - 1]->makePixmap(lastPixmap, width(), height());
  coolDown = 20;
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
