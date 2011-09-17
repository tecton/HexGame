#include "gamerecord.h"

#include <QDir>
#include <QFile>

GameRecord::GameRecord()
{
  // Set the path
  recordPath = QDir::homePath() + "/.HexGame/";
  QDir *dir = new QDir();

  // Make the path if it hasn't be created
  dir->mkpath(recordPath);
}

bool GameRecord::createFile(const QString &filename, int intCount)
{
  // Create the file
  QFile recordFile(recordPath + filename);
  if (!recordFile.open(QIODevice::WriteOnly))
    return false;

  // Set the values to 0
  QDataStream out(&recordFile);
  for (int i = 0 ; i < intCount; ++i)
    out << 0;

  return true;
}

bool GameRecord::resizeFile(const QString &filename, int intCount)
{
  // Resize the file
  QFile recordFile(recordPath + filename);
  return recordFile.resize(intCount * 4);
}

bool GameRecord::exists(const QString &filename)
{
  return QFile::exists(recordPath + filename);
}

bool GameRecord::remove(const QString &filename)
{
  return QFile::remove(recordPath + filename);
}

int GameRecord::sizeOfInt(const QString &filename)
{
  QFile recordFile(recordPath + filename);
  return recordFile.size() / sizeof(int);
}

bool GameRecord::writeData(const QString &filename, int pos, int data)
{
  //TODO
  // Open the file
  QFile recordFile(recordPath + filename);
  if (!recordFile.open(QIODevice::ReadWrite))
    return false;

  // Write the data
  QDataStream out(&recordFile);
  recordFile.seek(pos * sizeof(int));
  out << data;

  return true;
}

int GameRecord::readData(const QString &filename, int pos)
{
  //TODO
  // Open the file
  QFile recordFile(recordPath + filename);
  if (!recordFile.open(QIODevice::ReadOnly))
    return false;

  // Read the data
  QDataStream in(&recordFile);
  recordFile.seek(pos * sizeof(int));
  int result;
  in >> result;
  return result;
}

bool GameRecord::writeDataArr(const QString &filename, int *dataArr, int size)
{
  //TODO
  // Open the file
  QFile recordFile(recordPath + filename);
  if (!recordFile.open(QIODevice::WriteOnly))
    return false;

  // Write the data
  QDataStream out(&recordFile);
  for (int i = 0; i < size; ++i)
    out << dataArr[i];

  return true;
}

bool GameRecord::readDataArr(const QString &filename, int *&dataArr, int &size)
{
  //TODO
  // Open the file
  QFile recordFile(recordPath + filename);
  if (!recordFile.open(QIODevice::ReadOnly))
    return false;

  // Prepare space for the data
  size = sizeOfInt(filename);
  dataArr = new int[size];

  // Read the data
  QDataStream in(&recordFile);
  for (int i = 0; i < size; ++i)
    in >> dataArr[i];

  return true;
}
