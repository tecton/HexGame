/*
** Abstract class of an item.
** It should have the ability to paint itself.
*/

#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

class QPainter;

class AbstractItem
{
public:
  // Paint the item
  //
  // Input:
  //   painter:
  //     The QPainter to do the paint.
  //   width, height:
  //     The width and height of the whole QPaintDevice.
  //     (In the program it will be QPixmap)
  //   frame:
  //     The index of the frame to show.
  //     (An item may have lots of frames which
  //      could realize an animation)
  virtual void paint(QPainter *painter,
                     int width,
                     int height,
                     int frame)=0;
};

#endif // ABSTRACTITEM_H
