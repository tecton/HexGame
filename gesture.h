#ifndef GESTURE_H
#define GESTURE_H

#include "abstractrule.h"

class Gesture
{
public:
  Gesture(AbstractRule::Gesture theGestureType, int theIndex, int theOffset) :
      gestureType(theGestureType),
      index(theIndex),
      offset(theOffset)
  {}

  AbstractRule::Gesture getGestureTyp()
  {return gestureType;}

  int getIndex()
  {return index;}

  int getOffset()
  {return offset;}

private:
  AbstractRule::Gesture gestureType;

  int index;
  int offset;
};


#endif // GESTURE_H
