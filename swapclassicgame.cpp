#include "swapclassicgame.h"

#include <QPainter>
#include <QPixmap>
#include <QPointF>
#include <QTimer>
#include "effectpainter.h"
#include "rules.h"
#include "sixtyonegameboardinfo.h"
#include "connections.h"
#include "corecontroller.h"
#include "gesturecontroller.h"
#include "basicpainter.h"

SwapClassicGame::SwapClassicGame()
{
  rule = new SwapClassicGameRule();//SwapClassicGameRule();
  gameboardInfo = new SixtyOneGameBoardInfo();
  //SwapClassicGameSavedInfo savedInfo = readSaved();
  // 拿到其中的balls给下面那个的第三个参数，然后还有各种别的地方的值的改动
  controller = new CoreController(rule, gameboardInfo, NULL);
  controller->fillAllBlanks();
  gestureController = new GestureController(rule, gameboardInfo, controller);

  effectPainter = new EffectPainter(gameboardInfo);

  connect(controller,
          SIGNAL(stableEliminateTested(Connections)),
          this,
          SLOT(dealStableEliminate(Connections)));
  connect(controller,
          SIGNAL(userMovingEliminateTested(Connections)),
          this,
          SLOT(dealUserMovingEliminate(Connections)));

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();
}

void SwapClassicGame::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
  addEffect(pixmap, width, height);
}

SwapClassicGame::~SwapClassicGame()
{
  t->stop();
  delete t;
  delete controller;
  delete gameboardInfo;
  delete rule;
  delete gestureController;
  delete effectPainter;
}

//void SwapClassicGame::init() // 可能不需要饿
//{
//}

void SwapClassicGame::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  Ball **balls = controller->balls;
  BasicPainter::paintBasicBalls(balls,
                                gameboardInfo->totalBallCounts(),
                                painter,
                                width * 1.0 / gameboardInfo->width(),
                                height * 1.0 / gameboardInfo->height(),
                                frameCount);
  painter->end();
  delete painter;
}

void SwapClassicGame::addEffect(QPixmap& pixmap, int width, int height)
{
  QPainter *painter = new QPainter(&pixmap);
  effectPainter->paint(painter,
                       width * 1.0 / gameboardInfo->width(),
                       height * 1.0 / gameboardInfo->height());
  painter->end();
  delete painter;
}

QPointF SwapClassicGame::toScene(double xRate, double yRate)
{
  return QPointF(xRate * gameboardInfo->width(),
                 yRate * gameboardInfo->height());
}

//SwapClassicGameSavedInfo SwapClassicGame::readSaved()
//{

//}
void SwapClassicGame::quitGame()
{
  emit giveControlTo(NULL, true);
  delete this;
}

void SwapClassicGame::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  if (button == Qt::RightButton)
  {
    quitGame();
    return;
  }
  gestureController->dealPressed(mousePos);
}

void SwapClassicGame::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
  gestureController->dealMoved(mousePos);
}

void SwapClassicGame::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
  gestureController->dealReleased(mousePos);
}

void SwapClassicGame::advance()
{
  ++frameCount;
  frameCount = frameCount % 32;
  controller->advance();
//  effectPainter->advance();
}

void SwapClassicGame::dealStableEliminate(Connections connections)
{
  for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
  {
    QVector<QVector<int> *>& connect = connections.connectionsOfIndex[i];
    int connectionCountOfThePosition = 0;
    for (int i = 0;i < 10;++i)
    {
      if (i == 3 || connect[i] == NULL)
        continue;
      ++connectionCountOfThePosition;
    }
    if (connectionCountOfThePosition > 1)
    {
      effectPainter->highlightAt(i);

      // TODO:BLABLABLA 各种奖励，各种特效

    }
  }
}

void SwapClassicGame::dealUserMovingEliminate(Connections connections)
{
  for (int i = 0;i < connections.connections.size();++i)
  {
    for (int j = 0;j < connections.connections[i]->size();++j)
      effectPainter->eliminationHintAt(j);
    // TODO:BLABLABLA 提示
  }
}

