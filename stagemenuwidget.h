#ifndef PUZZLEUNITESTAGE_H
#define PUZZLEUNITESTAGE_H

#include "abstractpixmapwidget.h"

#include <QVector>

class QTimer;
class AbstractItem;
class StageMenuItem;

/**
 * @brief An abstract class of stage menu widgets.
 */
class AbstractStageMenuWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  // similar to other widgets
  virtual void makePixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
      int width,
      int height)=0;
  virtual void makeBasicPixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
      int width,
      int height)=0;
  virtual void addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
      int width,
      int height)=0;
  virtual QPointF toScene(double xRate, double yRate)=0;
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button)=0;
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button)=0;
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button)=0;
  virtual void getForcus()=0;
  QString prefix;
  QString suffix;
  QString imageName;
  int type;
};

/**
 * @brief A class of stage menu widget of exchange serial.
 */
class ExchangeStageMenuWidget : public AbstractStageMenuWidget
{
  Q_OBJECT
public:
  ExchangeStageMenuWidget(int stageType);
  ~ExchangeStageMenuWidget();

  virtual void makePixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                          int width,
                          int height);
  virtual void makeBasicPixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                               int width,
                               int height);
  virtual void addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                         int width,
                         int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);
  virtual void getForcus();

private:
  QTimer *t;
  StageMenuItem **stageItem;
  QVector <AbstractItem *> myItems;
  int frameCount;

private slots:
  void advance();
};

/**
 * @brief A class of stage menu widget of sunit serial.
 */
class UniteStageMenuWidget : public AbstractStageMenuWidget
{
  Q_OBJECT
public:
  UniteStageMenuWidget(int stageType);
  ~UniteStageMenuWidget();

  virtual void makePixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                          int width,
                          int height);
  virtual void makeBasicPixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                               int width,
                               int height);
  virtual void addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                               int width,
                               int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);
  virtual void getForcus();

private:
  QTimer *t;
  StageMenuItem **stageItem;
  QVector <AbstractItem *> myItems;
  int frameCount;

private slots:
  void advance();
};

/**
 * @brief A class of stage menu widget of lock serial.
 */
class LockStageMenuWidget : public AbstractStageMenuWidget
{
  Q_OBJECT
public:
  LockStageMenuWidget(int stageType);
  ~LockStageMenuWidget();

  virtual void makePixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                               int width,
                               int height);
  virtual void makeBasicPixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                               int width,
                               int height);
  virtual void addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                               int width,
                               int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);
  virtual void getForcus();

private:
  QTimer *t;
  StageMenuItem **stageItem;
  QVector <AbstractItem *> myItems;
  int frameCount;

private slots:
  void advance();
};
#endif // PUZZLEUNITESTAGE_H
