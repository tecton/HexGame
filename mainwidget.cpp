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
#include "config.h"
#include "achievements.h"
#include "achievementwidget.h"
#include "helpwidget2.h"

extern Achievements achievements;

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    coolDown(0)
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

/*  AbstractPixmapWidget **/mainMenu = new AchievementWidget();
//  mainMenu = new HelpWidget2();
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

  // Get the painter
  QPainter *painter = new QPainter(this);

  if (coolDown > 0)
  {
    // Paint last pixmap
    painter->drawPixmap(0,0,width(), height(), lastPixmap);
#ifdef USE_PIXMAP
    // Let the widget to make a pixmap
    widgets[widgets.size() - 1]->makePixmap(pixmap, width(), height());
    painter->drawPixmap(0,0,width(),height()-coolDown * height() / 20, pixmap, 0, coolDown * pixmap.height() / 20,pixmap.width(),(20-coolDown) * pixmap.height() / 20);
#else
    painter->translate(0, -coolDown * height() / 20);
    widgets[widgets.size() - 1]->makePixmap(painter, width(), height());
#endif
  }
  else
  {
#ifdef USE_PIXMAP
    // Let the widget to make a pixmap
    widgets[widgets.size() - 1]->makePixmap(pixmap, width(), height());

    painter->drawPixmap(0,0/*,width(), height()*/, pixmap);
#else
    widgets[widgets.size() - 1]->makePixmap(painter, width(), height());
#endif

  }

  achievements.paint(painter, width(), height());
  achievements.advance();

  // End the paint and release the space
  painter->end();
  delete painter;
}


QPointF MainWidget::toScene(QPointF mousePosition)
{
  double xRate = mousePosition.x() / width();
  double yRate = mousePosition.y() / height();
  return widgets[widgets.size() - 1]->toScene(xRate, yRate);
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
  if (coolDown > 0)
    return;

  // Tell the widget if neccessary
  QPointF pos = toScene(event->posF());
  widgets[widgets.size() - 1]->dealPressed(pos, event->button());

  event->accept();
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
  if (coolDown > 0)
    return;

  // Tell the widget if neccessary
  QPointF pos = toScene(event->posF());
  widgets[widgets.size() - 1]->dealMoved(pos, event->button());
  event->accept();
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (coolDown > 0)
    return;

  // Tell the widget if neccessary
  QPointF pos = toScene(event->posF());
  widgets[widgets.size() - 1]->dealReleased(pos, event->button());
  event->accept();
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
