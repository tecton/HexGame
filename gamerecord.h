#ifndef GAMERECORD_H
#define GAMERECORD_H

#include <QString>

class GameRecord
{
public:
  GameRecord();

  bool createFile(const QString &filename, int intCount);
  bool resizeFile(const QString &filename, int intCount);
<<<<<<< HEAD
  bool exists(const QString &filename);
  bool remove(const QString &filename);
  int size(const QString &filename);
=======
>>>>>>> 519221cf00496a29df0f0e8e7498d0891561c94a

  bool writeData(const QString &filename, int pos, int data);
  int readData(const QString &filename, int pos);

  bool writeDataArr(const QString &filename, int *dataArr, int size);
  bool readDataArr(const QString &filename, int *dataArr, int &size);

private:
  QString recordPath;
};

#endif // GAMERECORD_H
