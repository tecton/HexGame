#include "mainwidget.h"

#include <QApplication>
#include <QTimer>
#include <QPixmap>
#include <QPointF>
#include <QPainter>
#include <QPaintEvent>
//#include <QCursor>
#include <QPixmap>
#include "abstractpixmapwidget.h"
#include "mainmenuwidget.h"
#include "publicgamesounds.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    coolDown(0)
{
  // Set the curser(abandoned)
//  setCursor(QCursor(QPixmap(":/images/cursor.png")));
  // Create the main menu widget and push it into the stack
  MainMenuWidget *mainMenu = new MainMenuWidget();
  widgets.push_back(mainMenu);

  // Connect the signal and slot
  connect(mainMenu,
          SIGNAL(giveControlTo(AbstractPixmapWidget*,bool)),
          this,
          SLOT(changeControl(AbstractPixmapWidget*,bool)));

  // Create the timer
  refreshTimer = new QTimer();
  refreshTimer->setInterval(30);

  // Connect the signal and slot
  connect(refreshTimer, SIGNAL(timeout()),this, SLOT(update()));

  // Start the timer
  refreshTimer->start();
}

void MainWidget::paintEvent(QPaintEvent *event)
{
  // Calculate CD
  if (coolDown > 0)
  {
    --coolDown;
    if (coolDown == 0)
      // Tell the widget to get focus if neccessary
      widgets[widgets.size() - 1]->getForcus();
  }

  // Let the widget to make a pixmap
  widgets[widgets.size() - 1]->makePixmap(pixmap, width(), height());

  // Get the painter
  QPainter *painter = new QPainter(this);

  if (coolDown > 0)
  {
    // Paint with 2 pixmaps
    painter->drawPixmap(0,0,width(), height(), lastPixmap);
    painter->drawPixmap(0,0,width(),height()-coolDown * height() / 20, pixmap, 0, coolDown * pixmap.height() / 20,pixmap.width(),(20-coolDown) * pixmap.height() / 20);
  }
  else
    // Paint with 1 pixmap
    painter->drawPixmap(0,0/*,width(), height()*/, pixmap);

  // End the paint and release the space
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

  // Tell the widget if neccessary
  QPointF pos = toScene(event->posF());
  widgets[widgets.size() - 1]->dealPressed(pos, event->button());
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
  if (coolDown > 0)
    return;

  // Tell the widget if neccessary
  QPointF pos = toScene(event->posF());
  widgets[widgets.size() - 1]->dealMoved(pos, event->button());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (coolDown > 0)
    return;

  // Tell the widget if neccessary
  QPointF pos = toScene(event->posF());
  widgets[widgets.size() - 1]->dealReleased(pos, event->button());
}

void MainWidget::changeControl(AbstractPixmapWidget *target,
                               bool deleteMySelf)
{
  // Record the last pixmap
  widgets[widgets.size() - 1]->makePixmap(lastPixmap, width(), height());

  // Set CD
  coolDown = 20;

  // Pop
  if (deleteMySelf)
    widgets.pop_back();

  // Clear the sound effect
  if (deleteMySelf && !target)
    PublicGameSounds::clear();

  // Push
  if (target)
  {
    widgets.push_back(target);

    // Connect the signal and slot
    connect(target,
            SIGNAL(giveControlTo(AbstractPixmapWidget*,bool)),
            this,
            SLOT(changeControl(AbstractPixmapWidget*,bool)));
  }
  // Exit
  if (widgets.isEmpty())
  {
    qApp->exit();
//    delete this;
    //something wrong with it, even I write ~MainWidget, it still goes wrong
  }
}
