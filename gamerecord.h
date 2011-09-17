/*
** Class to deal with the files.
*/

#ifndef GAMERECORD_H
#define GAMERECORD_H

// File must include
#include <QString>

class GameRecord
{
public:
  // Constructor
  GameRecord();

  // Create a file
  bool createFile(const QString &filename, int intCount);

  // Resize a file
  bool resizeFile(const QString &filename, int intCount);

  // Whether a file exists
  bool exists(const QString &filename);

  // Remove a file
  bool remove(const QString &filename);

  // Count of integer in the file
  int sizeOfInt(const QString &filename);

  // Write data
  bool writeData(const QString &filename, int pos, int data);

  // Read data
  int readData(const QString &filename, int pos);

  // Write data in an array
  bool writeDataArr(const QString &filename, int *dataArr, int size);

  // Read data into an array
  bool readDataArr(const QString &filename, int *&dataArr, int &size);

private:
  // The path if the record
  QString recordPath;
};

#endif // GAMERECORD_H
