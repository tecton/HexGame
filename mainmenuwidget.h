#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H

#include "abstractpixmapwidget.h"

#include <QVector>

class QTimer;
class AbstractItem;
class MainMenuGameItem;

class MainMenuWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  MainMenuWidget();
  ~MainMenuWidget();

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
  MainMenuGameItem *items[9];
  QVector <AbstractItem *> myItems;
  int frameCount;

private slots:
  void advance();
};

#endif // MAINMENUWIDGET_H
