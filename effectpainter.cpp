#include "effectpainter.h"

#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QPolygonF>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QPixmap>
#include "abstractgameboardinfo.h"
#include "gamemath.h"
#include "pixmapoperations.h"

// An abstract class of effects
class AbstractEffect
{
public:
  // A pure virtual function to paint the effect on the screen
  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)=0;
};

// An abstract class of effects only last for a while
class AbstractAgingEffect : public AbstractEffect
{
public:
  // Function most overloaded(still a pure virtual one)
  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)=0;

  // Advance the effect, returns whether it should last
  inline bool advance()
  {
    ++age;
    return age <= ageLimit;
  }

  // Getter and setter of the 2 properties
  inline int getAge()
  {return age;}

  inline void setAge(int v)
  {age = v;}

  inline int getLimit()
  {return ageLimit;}

  inline void setLimit(int v)
  {ageLimit = v;}

private:
  // The current age of the effect
  int age;

  // The limit of the age of the effect
  int ageLimit;
};

// A class of an effect to show which balls were choosed
class SelectionHint : public AbstractLongLastingEffect
{
public:
  // Constructor with the area of the effect
  SelectionHint(QRectF position) :
      pos(position)
  {}

  // Getter and setter of the position
  inline void setPos(QRectF position)
  {pos = position;}

  inline QRectF getPos()
  {return pos;}

  // Function most overloaded
  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)
  {
    // Set the pen
    QPen pen = QPen(QColor(255,  0,  0,150));
    pen.setWidth(5 * xRate);
    painter->setBrush(QBrush(QColor(255,255,255,0)));
    painter->setPen(pen);

    // Calculate the polygon to paint
    QVector<QPointF> points;
    points.push_back(QPointF(xRate * pos.left(),
                             yRate * (3 * pos.top() + pos.bottom()) / 4));
    points.push_back(QPointF(xRate * (pos.left() + pos.right()) / 2,
                             yRate * pos.top()));
    points.push_back(QPointF(xRate * pos.right(),
                             yRate * (3 * pos.top() + pos.bottom()) / 4));
    points.push_back(QPointF(xRate * pos.right(),
                             yRate * (pos.top() + 3 * pos.bottom()) / 4));
    points.push_back(QPointF(xRate * (pos.left() + pos.right()) / 2,
                             yRate * pos.bottom()));
    points.push_back(QPointF(xRate * pos.left(),
                             yRate * (pos.top() + 3 * pos.bottom()) / 4));
    QPolygonF poly(points);

    // Paint the polygon
    painter->drawPolygon(poly);
  }

private:
  // The area of the effect
  QRectF pos;
};

// A class of an effect to show which balls will be eliminated after the user
// release the mouse
class EliminationHint : public AbstractLongLastingEffect
{
public:
  // Constructor with the area of the effect
  EliminationHint(QRectF position) :
      pos(position)
  {}

  // Getter and setter of the position
  inline void setPos(QRectF position)
  {pos = position;}

  inline QRectF getPos()
  {return pos;}

  // Function most overloaded
  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)
  {
    // Set the brush and pen
    painter->setBrush(QBrush(QColor(255,255,255,100)));
    painter->setPen(QPen(QColor(255,255,255,100)));

    // Calculate the polygon to paint
    QVector<QPointF> points;
    points.push_back(QPointF(xRate * pos.left(),
                             yRate * (3 * pos.top() + pos.bottom()) / 4));
    points.push_back(QPointF(xRate * (pos.left() + pos.right()) / 2,
                             yRate * pos.top()));
    points.push_back(QPointF(xRate * pos.right(),
                             yRate * (3 * pos.top() + pos.bottom()) / 4));
    points.push_back(QPointF(xRate * pos.right(),
                             yRate * (pos.top() + 3 * pos.bottom()) / 4));
    points.push_back(QPointF(xRate * (pos.left() + pos.right()) / 2,
                             yRate * pos.bottom()));
    points.push_back(QPointF(xRate * pos.left(),
                             yRate * (pos.top() + 3 * pos.bottom()) / 4));
    QPolygonF poly(points);

    // Paint the polygon
    painter->drawPolygon(poly);
  }

private:
  // The area of the effect
  QRectF pos;
};

// A class of an effect to show an explosion
class ExplodeInfo : public AbstractAgingEffect
{
public:
  // Constructor with the center position of the effect
  ExplodeInfo(QPointF position) :
      pos(position)
  {
    setAge(0);
    setLimit(5);
  }

  // Getter and setter of the position
  inline void setPos(QPointF position)
  {pos = position;}

  inline QPointF getPos()
  {return pos;}

  // Function most overloaded
  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)
  {
    // Calculate some values to locate
    double r = 3 * theGameboardInfo->ballR() *
               getAge() /
               getLimit();
    QPointF pos2 = scale(pos, xRate, yRate);

    // Set the brush and pen
    QRadialGradient gradient = QRadialGradient(pos2, r, pos2);
    gradient.setColorAt(0, QColor(255,255,255,200));
    gradient.setColorAt(1, QColor(255,255,255,50));
    painter->setPen(QColor(255,255,255,0));
    painter->setBrush(QBrush(gradient));

    // Draw the ellipse
    painter->drawEllipse(pos2, r, r);
  }

private:
  // Center of the explosion
  QPointF pos;
};

// 2 arrays used to locate in the following effect
double kkx[3] = {-0.5, 0.5, 1};
double kky[3] = {-0.866, -0.866, 0};

// A class of an effect to show a lightning in 6 directions
class LightningInfo : public AbstractAgingEffect
{
public:
  // Constructor with the center position of the effect
  LightningInfo(QPointF position) :
      pos(position)
  {
    // Calculate some points to locate
    linearPoints[0] = QPointF(pos.x() - 7.5 * 1.732, pos.y() + 7.5);
    linearPoints[1] = QPointF(pos.x() + 7.5 * 1.732, pos.y() + 7.5);
    linearPoints[2] = QPointF(pos.x(), pos.y() + 15);
    setAge(0);
    setLimit(5);
  }

  // Getter and setter of the position
  inline void setPos(QPointF position)
  {
    pos = position;
    linearPoints[0] = QPointF(pos.x() - 7.5 * 1.732, pos.y() + 7.5);
    linearPoints[1] = QPointF(pos.x() + 7.5 * 1.732, pos.y() + 7.5);
    linearPoints[2] = QPointF(pos.x(), pos.y() + 15);
  }

  inline QPointF getPos()
  {return pos;}

  // Function most overloaded
  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)
  {
    // Calculate some values to locate
    QPointF pos2 = scale(pos, xRate, yRate);
    double r = distanceOfTwoPoints(QPointF(0, 0),
                                   QPointF(theGameboardInfo->width(),
                                           theGameboardInfo->height())) *
               getAge() /
               getLimit() /
               2;

    // Draw the effect into 3 * 2 directions
    for (int i = 0;i < 3;++i)
    {
      // Set the brush and pen
      QLinearGradient gradient = QLinearGradient(pos2,
                                                 scale(linearPoints[i],
                                                       xRate,
                                                       yRate));
      gradient.setColorAt(0, QColor(255,255,255,255));
      gradient.setColorAt(1, QColor(255,255,255,100));
      gradient.setSpread(QGradient::ReflectSpread);
      QPen pen = QPen(QBrush(gradient), 30);
      painter->setPen(pen);
      QPointF posA = QPointF(pos2.x() + kkx[i] * r, pos2.y() + kky[i] * r);
      QPointF posB = QPointF(pos2.x() - kkx[i] * r, pos2.y() - kky[i] * r);

      // Draw effect into 2 directions
      painter->drawLine(posA, posB);
    }
  }

private:
  // Center of the lightning
  QPointF pos;

  // Some points to locate
  QPointF linearPoints[3];
};

// A class of an effect to show a highlight
class HighlightInfo : public AbstractAgingEffect
{
public:
  // Constructor with the center position of the effect
  HighlightInfo(QPointF position) :
      pos(position)
  {
    setAge(0);
    setLimit(10);
  }

  // Getter and setter of the position
  inline void setPos(QPointF position)
  {pos = position;}

  inline QPointF getPos()
  {return pos;}

  // Function most overloaded
  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)
  {
    // Calculate some values to locate
    QPointF pos2 = scale(pos, xRate, yRate);
    double r = theGameboardInfo->intervalBetweenTwoLayers() *
               getAge() /
               getLimit();

    // Set the brush and pen
    QColor color = QColor(255,255,255,100 + 50 * getAge() / getLimit());
    QRadialGradient gradient = QRadialGradient(pos2, r, pos2);
    gradient.setColorAt(0, color);
    gradient.setColorAt(1, QColor(255, 255, 255, 50));
    painter->setPen(QColor(255, 255, 255, 0));
    painter->setBrush(QBrush(gradient));

    // Draw the ellipse
    painter->drawEllipse(pos2, r, r);
  }

private:
  // Center of the highlight
  QPointF pos;
};

// A class of an effect to show words
class WordsInfo : public AbstractAgingEffect
{
public:
  // Constructor with the words, center position, size of the words
  WordsInfo(QString str, QPointF position, double size) :
      string(str),
      pos(position)
  {
    f.setPointSize(size * 8);
    setAge(0);
    setLimit(20);
  }

  // Getter and setter of the position
  inline void setPos(QPointF position)
  {pos = position;}

  inline QPointF getPos()
  {return pos;}

  // Function most overloaded
  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)
  {
    // Save the font
    QFont originalFont = painter->font();

    // Calculate the left top position of the text
    QPointF pos2 = pos;
    pos2.setY(pos2.y() - getAge());
    pos2 = scale(pos2, xRate, yRate);

    // Set the font and pen
    painter->setPen(QColor(255, 255, 255, 100 + 50 * getAge() / getLimit()));
    painter->setFont(f);

    // Draw the words
    painter->drawText(pos2, string);

    // Set the font to the original one
    painter->setFont(originalFont);
  }

private:
  // The words
  QString string;

  // The position of the words
  QPointF pos;

  // The font of the words
  QFont f;
};

// A class of an effect to show hints
class HintInfo : public AbstractAgingEffect
{
public:
  // Constructor with the center position, whether it's rotate
  HintInfo(QPointF position, bool rotate) :
      pos(position),
      r(rotate)
  {
    // Set the correct pixmap
    if (rotate)
      p = QPixmap(":/images/hint/rotate.png");
    else
      p = QPixmap(":/images/hint/arrow.png");
    setAge(0);
    setLimit(20);
  }

  // Getter and setter of the position
  inline void setPos(QPointF position)
  {pos = position;}

  inline QPointF getPos()
  {return pos;}

  // Function most overloaded
  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)
  {
    // Calculate the values to locate
    QPointF pos2 = pos;
    double dy;
    double percentage = 1.0 * getAge() / getLimit();
    if (percentage < 0.25)
      dy = 10 - 40 * percentage;
    else if (percentage >= 0.25 && percentage < 0.5)
      dy = 40 * (percentage - 0.25);
    else if (percentage >= 0.5 && percentage < 0.75)
      dy = 10 - 40 * (percentage - 0.5);
    else if (percentage >= 0.75 && percentage < 1)
      dy = 40 * (percentage - 0.75);
    else
      dy = 10;
    if (!r)
      dy += theGameboardInfo->ballR();
    pos2.setY(pos2.y() - dy);
    pos2 = scale(pos2, xRate, yRate);

    // Draw the pixmap
    drawPixmapAt(painter,p,xRate,yRate,pos2,true,true);
  }

private:
  // The center position of the hint
  QPointF pos;

  // Whether it's rotate
  bool r;

  // The pixmap of the hint
  QPixmap p;
};

// A class of an effect to show a flash (full screen)
class FlashInfo : public AbstractAgingEffect
{
public:
  // Constructor
  FlashInfo()
  {
    setAge(0);
    setLimit(12);
  }

  // Function most overloaded
  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)
  {
    // Set the brush and pen
    QColor color(255, 255, 255, getAge() % 4 * 30);
    painter->setPen(color);
    painter->setBrush(color);

    // Draw the rectangle
    painter->drawRect(0,
                      0,
                      theGameboardInfo->width() * xRate,
                      theGameboardInfo->height() * yRate);
  }
};

EffectPainter::EffectPainter(
    AbstractGameBoardInfo *theGameboardInfo) :
    gameboardInfo(theGameboardInfo)
{
}

void EffectPainter::selectAt(int index)
{
  // Calculate the area
  QPointF center = gameboardInfo->positionOfIndex(index);
  QPointF leftTop = QPointF(center.x() - gameboardInfo->intervalBetweenTwoLayers(),
                            center.y() - 1.155 * gameboardInfo->intervalBetweenTwoLayers());
  QPointF rightBottom = QPointF(center.x() + gameboardInfo->intervalBetweenTwoLayers(),
                                center.y() + 1.155 * gameboardInfo->intervalBetweenTwoLayers());
  QRectF rect = QRectF(leftTop, rightBottom);

  // Create the effect and record it
  SelectionHint *hint = new SelectionHint(rect);
  selectionEffects.push_back(hint);
}

void EffectPainter::clearSelectionHints()
{
  // Delete the effects
  for (int i = 0;i < selectionEffects.size();++i)
    delete selectionEffects[i];
  selectionEffects.clear();
}

void EffectPainter::bonusEliminationHintAt(int index)
{
  // Calculate the area
  QPointF center = gameboardInfo->positionOfIndex(index);
  QPointF leftTop = QPointF(center.x() - gameboardInfo->intervalBetweenTwoLayers(),
                            center.y() - 1.155 * gameboardInfo->intervalBetweenTwoLayers());
  QPointF rightBottom = QPointF(center.x() + gameboardInfo->intervalBetweenTwoLayers(),
                                center.y() + 1.155 * gameboardInfo->intervalBetweenTwoLayers());
  QRectF rect = QRectF(leftTop, rightBottom);

  // Create the effect and record it
  EliminationHint *hint = new EliminationHint(rect);
  bonusEliminateEffects.push_back(hint);
}

void EffectPainter::clearBonusEliminationHints()
{
  // Delete the effects
  for (int i = 0;i < bonusEliminateEffects.size();++i)
    delete bonusEliminateEffects[i];
  bonusEliminateEffects.clear();
}

void EffectPainter::userMovingEliminationHintAt(int index)
{
  // Calculate the area
  QPointF center = gameboardInfo->positionOfIndex(index);
  QPointF leftTop = QPointF(center.x() - gameboardInfo->intervalBetweenTwoLayers(),
                            center.y() - 1.155 * gameboardInfo->intervalBetweenTwoLayers());
  QPointF rightBottom = QPointF(center.x() + gameboardInfo->intervalBetweenTwoLayers(),
                                center.y() + 1.155 * gameboardInfo->intervalBetweenTwoLayers());
  QRectF rect = QRectF(leftTop, rightBottom);

  // Create the effect and record it
  EliminationHint *hint = new EliminationHint(rect);
  userMovingEliminateEffects.push_back(hint);
}

void EffectPainter::clearUserMovingEliminationHints()
{
  // Delete the effects
  for (int i = 0;i < userMovingEliminateEffects.size();++i)
    delete userMovingEliminateEffects[i];
  userMovingEliminateEffects.clear();
}

void EffectPainter::explodeAt(int index)
{
  // Create the effect and record it
  ExplodeInfo *info = new ExplodeInfo(gameboardInfo->positionOfIndex(index));
  agingEffects.push_back(info);
}

void EffectPainter::lightningAt(int index/*, QVector<int> directions*/)
{
  LightningInfo *info = new LightningInfo(gameboardInfo->positionOfIndex(index));
  agingEffects.push_back(info);
}

void EffectPainter::highlightAt(int index)
{
  // Calculate the position
  QPointF center = gameboardInfo->positionOfIndex(index);

  // Create the effect and record it
  HighlightInfo *info = new HighlightInfo(center);
  agingEffects.push_back(info);
}

void EffectPainter::wordsAt(QPointF pos, QString str, double size)
{
  // Create the effect and record it
  WordsInfo *info = new WordsInfo(str, pos, size);
  agingEffects.push_back(info);
}

void EffectPainter::flash()
{
  // Create the effect and record it
  FlashInfo *info = new FlashInfo();
  agingEffects.push_back(info);
}

void EffectPainter::hintAt(QPointF pos, bool rotate)
{
  // Create the effect and record it
  HintInfo *info = new HintInfo(pos, rotate);
  agingEffects.push_back(info);
}

//void EffectPainter::highlightAll()
//{
//  HighlightInfo info;
//  info.age = 0;
//  QPointF leftTop = QPointF(0, 0);
//  QPointF rightBottom = QPointF(gameboardInfo->width(),
//                                gameboardInfo->height());
//  QRectF rect = QRectF(leftTop, rightBottom);
//  info.area = rect;
//  highlightInfos.push_back(info);
//}

void EffectPainter::clearEffects()
{
  // Clear all the effects
  clearSelectionHints();
  clearBonusEliminationHints();
  clearUserMovingEliminationHints();
  for (int i = 0;i < agingEffects.size();++i)
    delete agingEffects[i];
  agingEffects.clear();
}

void EffectPainter::advance()
{
  // Advance the aging effects
  AbstractAgingEffect *agingEffect;
  foreach (agingEffect, agingEffects)
  {
    if (!agingEffect->advance())
    {
      delete agingEffect;
      agingEffects.removeOne(agingEffect);
    }
  }
}

void EffectPainter::paint(QPainter *painter,
                          double xRate,
                          double yRate)
{
  // Paint the effects one by one
  AbstractEffect *effect;
  foreach (effect, agingEffects)
    effect->paint(gameboardInfo, painter, xRate, yRate);

  foreach (effect, selectionEffects)
    effect->paint(gameboardInfo, painter, xRate, yRate);

  foreach (effect, bonusEliminateEffects)
    effect->paint(gameboardInfo, painter, xRate, yRate);

  foreach (effect, userMovingEliminateEffects)
    effect->paint(gameboardInfo, painter, xRate, yRate);
}
