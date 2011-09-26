#ifndef PUZZLEGAMERECORD_H
#define PUZZLEGAMERECORD_H

class PuzzleGameRecord
{
public:
  static void getColorIndexes(int type,
                              int index,
                              int *&ballIndex,
                              int *&toBeIndex,
                              int& count);

  static int minSteps(int type, int index);

  static void testLeastSteps(int type, int index, int step);

  static void minSteps(int type, int *&array, int& count);

  static int clearedStages(int type);

  static int clearedStages();

  static int totalStages(int type);

  static int totalStages();
};

#endif // PUZZLEGAMERECORD_H
