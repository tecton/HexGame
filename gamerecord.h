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
  /**
   * @brief Constructor.
   */
  GameRecord();

  /**
   * @brief Create a file.
   */
  bool createFile(const QString &filename, int intCount);

  /**
   * @brief Resize a file.
   */
  bool resizeFile(const QString &filename, int intCount);

  /**
   * @brief Whether a file exists.
   */
  bool exists(const QString &filename);

  /**
   * @brief Remove a file.
   */
  bool remove(const QString &filename);

  /**
   * @brief Count of integer in the file.
   */
  int sizeOfInt(const QString &filename);

  /**
   * @brief Write data.
   */
  bool writeData(const QString &filename, int pos, int data);

  /**
   * @brief Read data.
   */
  int readData(const QString &filename, int pos);

  /**
   * @brief Write data in an array.
   */
  bool writeDataArr(const QString &filename, int *dataArr, int size);

  /**
   * @brief Read data into an array.
   */
  bool readDataArr(const QString &filename, int *&dataArr, int &size);

private:
  // The path if the record
  QString recordPath;
};

#endif // GAMERECORD_H
