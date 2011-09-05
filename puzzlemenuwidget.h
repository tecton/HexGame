#ifndef PUZZLEMENUWIDGET_H
#define PUZZLEMENUWIDGET_H

#include "abstractpixmapwidget.h"

#include <QVector>

class QTimer;
class AbstractItem;
class AbstractPuzzleMenuItem;

class PuzzleMenuWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  PuzzleMenuWidget();
  ~PuzzleMenuWidget();

  virtual void makePixmap(QPixmap& pixmap, int width, int height);
  virtual void makeBasicPixmap(QPixmap& pixmap, int width, int height);
  virtual void addEffect(QPixmap& pixmap, int width, int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);
  virtual void getForcus(){}

private:
  QTimer *t;
  AbstractPuzzleMenuItem *exchangeItem;
  AbstractPuzzleMenuItem *uniteItem;
  AbstractPuzzleMenuItem *lockItem;
  AbstractItem *exitItem;
  QVector <AbstractItem *> myItems;
  int frameCount;

private slots:
  void advance();
};

#endif // PUZZLEMENUWIDGET_H
