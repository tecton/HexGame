#ifndef GAMERECORD_H
#define GAMERECORD_H

#include <QString>

class GameRecord
{
public:
  GameRecord();

  bool createFile(const QString &filename, int intCount);
  bool resizeFile(const QString &filename, int intCount);

  bool writeData(const QString &filename, int pos, int data);
  int readData(const QString &filename, int pos);

  bool writeDataArr(const QString &filename, int *dataArr, int size);
  bool readDataArr(const QString &filename, int *dataArr, int &size);

private:
  QString recordPath;
};

#endif // GAMERECORD_H
