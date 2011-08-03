#ifndef PUZZLEUNITESTAGE_H
#define PUZZLEUNITESTAGE_H

#include "abstractpixmapwidget.h"

#include <QVector>

class QTimer;
class AbstractItem;
class StageMenuItem;

class AbstractStageMenuWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  virtual void makePixmap(QPixmap& pixmap, int width, int height)=0;
  virtual void makeBasicPixmap(QPixmap& pixmap, int width, int height)=0;
  virtual void addEffect(QPixmap& pixmap, int width, int height)=0;
  virtual QPointF toScene(double xRate, double yRate)=0;
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button)=0;
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button)=0;
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button)=0;
};

class ExchangeStageMenuWidget : public AbstractStageMenuWidget
{
  Q_OBJECT
public:
  ExchangeStageMenuWidget();
  ~ExchangeStageMenuWidget();

  virtual void makePixmap(QPixmap& pixmap, int width, int height);
  virtual void makeBasicPixmap(QPixmap& pixmap, int width, int height);
  virtual void addEffect(QPixmap& pixmap, int width, int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);

private:
  QTimer *t;
  StageMenuItem **stageItem;
  QVector <AbstractItem *> myItems;
  int frameCount;

private slots:
  void advance();
};

class UniteStageMenuWidget : public AbstractStageMenuWidget
{
  Q_OBJECT
public:
  UniteStageMenuWidget();
  ~UniteStageMenuWidget();

  virtual void makePixmap(QPixmap& pixmap, int width, int height);
  virtual void makeBasicPixmap(QPixmap& pixmap, int width, int height);
  virtual void addEffect(QPixmap& pixmap, int width, int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);

private:
  QTimer *t;
  StageMenuItem **stageItem;
  QVector <AbstractItem *> myItems;
  int frameCount;

private slots:
  void advance();
};
#endif // PUZZLEUNITESTAGE_H
