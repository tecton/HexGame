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
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);
  virtual void getForcus()=0;
  virtual void loseForcus(){}

  /**
   * @brief Get the type(including the theme and advanced or not) of the stage
   * menu.
   */
  virtual int getType()=0;

  /**
   * @brief Switch the type(advanced or not) of the stage menu.
   */
  virtual AbstractStageMenuWidget *switchType(int currentType)=0;

  int stageCount;
  int type;
  StageMenuItem **stageItem;
};

/**
 * @brief A class of stage menu widget of exchange serial.
 */
class ExchangeStageMenuWidget : public AbstractStageMenuWidget
{
  Q_OBJECT
public:
  /**
   * @brief Constructor.
   */
  ExchangeStageMenuWidget(int stageType);

  /**
   * @brief Destructor.
   */
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
  virtual void getForcus();

  virtual int getType()
  {return type;}

  virtual AbstractStageMenuWidget *switchType(int currentType)
  {return new ExchangeStageMenuWidget(currentType ^ 1);}

private:
  QTimer *t;
  QVector <AbstractItem *> myItems;
  int frameCount;

private slots:
  void advance();
};

/**
 * @brief A class of stage menu widget of stage serial.
 */
class UniteStageMenuWidget : public AbstractStageMenuWidget
{
  Q_OBJECT
public:
  /**
   * @brief Constructor.
   */
  UniteStageMenuWidget(int stageType);

  /**
   * @brief Destructor.
   */
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
  virtual void getForcus();

  virtual int getType()
  {return type + 2;}

  virtual AbstractStageMenuWidget *switchType(int currentType)
  {return new UniteStageMenuWidget(currentType ^ 1);}

private:
  QTimer *t;
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
  /**
   * @brief Constructor.
   */
  LockStageMenuWidget(int stageType);

  /**
   * @brief Destructor.
   */
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
  virtual void getForcus();

  virtual int getType()
  {return type + 4;}

  virtual AbstractStageMenuWidget *switchType(int currentType)
  {return new LockStageMenuWidget(currentType ^ 1);}

private:
  QTimer *t;
  QVector <AbstractItem *> myItems;
  int frameCount;

private slots:
  void advance();
};
#endif // PUZZLEUNITESTAGE_H
