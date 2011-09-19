/*
** Abstract class of a widget which can't be seen directly, but can make a
** pixmap to show what it should look like. Only one such widget is allowed to
** be active at a time, so it should emit a signal when it give this ability
** to other widget.
*/

#ifndef ABSTRACTPIXMAPWIDGET_H
#define ABSTRACTPIXMAPWIDGET_H

// File must include
#include <QObject>

// Forward declaration
class QPixmap;
class QPointF;

class AbstractPixmapWidget :public QObject
{
  Q_OBJECT
public:
  /**
   *@brief Make the pixmap with the given width and height.
   *
   * Make everything of the pixmap.
   *@param pixmap The pixmap.
   *@param width The width of the pixmap should be.
   *@param height The height of the pixmap should be.
   */
  virtual void makePixmap(QPixmap& pixmap, int width, int height)=0;

  /**
   *@brief Make the basic part of the pixmap.
   *
   * Make only the basic part of the pixmap.
   *@param pixmap The pixmap.
   *@param width The width of the pixmap should be.
   *@param height The height of the pixmap should be.
   */
  virtual void makeBasicPixmap(QPixmap& pixmap, int width, int height)=0;

  /**
   *@brief Add the effect to the pixmap.
   *
   * Make the effect part of the pixmap.
   *@param pixmap The pixmap.
   *@param width The width of the pixmap should be.
   *@param height The height of the pixmap should be.
   */
  virtual void addEffect(QPixmap& pixmap, int width, int height)=0;

  /**
   *@brief Returns the logical position of the given rate.
   *
   * A widget can suppose it's width and height, and deal wiht the mouse events
   * according to the logical position.
   *@param xRate The scale in X direction.
   *@param yRate The scale in Y direction.
   *@return The logical position.
   */
  virtual QPointF toScene(double xRate, double yRate)=0;

  //@{
  /** 3 functions to deal with mouse operations. */
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button)=0;
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button)=0;
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button)=0;
  //@}

  /**
   *@brief Should be called when the widget get focus, used to restart the
   * timer(s).
   */
  virtual void getForcus()=0;

signals:
  /**
   *@brief A signal which tell the main widget to change the controller.
   */
  void giveControlTo(AbstractPixmapWidget *target, bool deleteMySelf);
};

#endif // ABSTRACTPIXMAPWIDGET_H
