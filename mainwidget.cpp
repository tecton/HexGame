#include "mainwidget.h"

#include <QApplication>
#include <QTimer>
#include <QPixmap>
#include <QPointF>
#include <QPainter>
#include <QPaintEvent>
//#include <QCursor>
#include <QPixmap>
#include <QTouchEvent>
#include <QMouseEvent>
#include "abstractpixmapwidget.h"
#include "mainmenuwidget.h"
#include "publicgamesounds.h"
#include "config.h"
#include "achievements.h"

extern Achievements achievements;

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    coolDown(0),
    canGetTouch(false)
{
  // Set the curser(abandoned)
//  setCursor(QCursor(QPixmap(":/images/cursor.png")));
  // Create the main menu widget and push it into the stack
  AbstractPixmapWidget *mainMenu = new MainMenuWidget();
  widgets.push_back(mainMenu);

  // Connect the signal and slot
  connect(mainMenu,
          SIGNAL(giveControlTo(AbstractPixmapWidget*,bool)),
          this,
          SLOT(changeControl(AbstractPixmapWidget*,bool)));

  mainMenu->getForcus();

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

  // Get the painter
  QPainter *painter = new QPainter(this);

  int widthToPaint = width();
  int heightToPaint = height();

  if (height() > width())
  {
    int tmp = widthToPaint;
    widthToPaint = heightToPaint;
    heightToPaint = tmp;
    painter->translate(0, height());
    painter->rotate(-90);
  }

  if (coolDown > 0)
  {
    // Paint last pixmap
    painter->drawPixmap(0,0,widthToPaint, heightToPaint, lastPixmap);
#ifdef USE_PIXMAP
    // Let the widget to make a pixmap
    widgets[widgets.size() - 1]->makePixmap(pixmap, widthToPaint, heightToPaint);
    painter->drawPixmap(0,0,widthToPaint,heightToPaint-coolDown * heightToPaint / 20, pixmap, 0, coolDown * pixmap.height() / 20,pixmap.width(),(20-coolDown) * pixmap.height() / 20);
#else
    painter->translate(0, -coolDown * height() / 20);
    widgets[widgets.size() - 1]->makePixmap(painter, widthToPaint, heightToPaint);
#endif
  }
  else
  {
#ifdef USE_PIXMAP
    // Let the widget to make a pixmap
    widgets[widgets.size() - 1]->makePixmap(pixmap, widthToPaint, heightToPaint);

    painter->drawPixmap(0,0/*,width(), height()*/, pixmap);
#else
    widgets[widgets.size() - 1]->makePixmap(painter, widthToPaint, heightToPaint);
#endif

  }

  achievements.paint(painter, widthToPaint, heightToPaint);
  achievements.advance();

  // End the paint and release the space
  painter->end();
  delete painter;
}


QPointF MainWidget::toScene(QPointF mousePosition)
{
  double xRate;
  double yRate;
  if (height() > width())
  {
    xRate = (height() - mousePosition.y()) / height();
    yRate = mousePosition.x() / width();
  }
  else
  {
    xRate = mousePosition.x() / width();
    yRate = mousePosition.y() / height();
  }
  return widgets[widgets.size() - 1]->toScene(xRate, yRate);
}

bool MainWidget::event(QEvent *event)
{
  switch (event->type())
  {
  case QEvent::TouchBegin:
  case QEvent::TouchUpdate:
  case QEvent::TouchEnd:
  {
    canGetTouch = true;
    if (coolDown > 0)
      break;
#ifndef Q_WS_MAC
    QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);

    QTouchEvent::TouchPoint touchPoint;

    foreach (touchPoint, touchEvent->touchPoints())
    {
      QPointF pos = toScene(touchPoint.pos());
      switch (touchPoint.state())
      {
      case Qt::TouchPointPressed:
        widgets[widgets.size() - 1]->dealPressed(toScene(pos), Qt::LeftButton);
        //event->accept();
        break;
      case Qt::TouchPointMoved:
        widgets[widgets.size() - 1]->dealMoved(toScene(pos), Qt::LeftButton);
        //event->accept();
        break;
      case Qt::TouchPointReleased:
        widgets[widgets.size() - 1]->dealReleased(toScene(pos), Qt::LeftButton);
        //event->accept();
        break;
      }
    }
#endif // Q_WS_MAC
    return true;
    break;
  }
  case QEvent::MouseButtonPress:
    if (canGetTouch)
      break;
    widgets[widgets.size() - 1]->dealPressed
        (toScene(((QMouseEvent*)event)->posF()),
         ((QMouseEvent*)event)->button());
    event->accept();
    return true;
    break;
  case QEvent::MouseMove:
    if (canGetTouch)
      break;
    widgets[widgets.size() - 1]->dealMoved
        (toScene(((QMouseEvent*)event)->posF()),
         ((QMouseEvent*)event)->button());
    event->accept();
    return true;
    break;
  case QEvent::MouseButtonRelease:
    if (canGetTouch)
      break;
    widgets[widgets.size() - 1]->dealReleased
        (toScene(((QMouseEvent*)event)->posF()),
         ((QMouseEvent*)event)->button());
    event->accept();
    return true;
    break;
  default:
    break;
  }
  return QWidget::event(event);
}

void MainWidget::changeControl(AbstractPixmapWidget *target,
                               bool deleteMySelf)
{
  // Record the last pixmap
#ifdef USE_PIXMAP
  widgets[widgets.size() - 1]->makePixmap(lastPixmap, width(), height());
#else
  lastPixmap = QPixmap(width(), height());

  QPainter *painter = new QPainter(&lastPixmap);
  widgets[widgets.size() - 1]->makePixmap(painter, width(), height());

  // End the paint and release the space
  painter->end();
  delete painter;
#endif

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
