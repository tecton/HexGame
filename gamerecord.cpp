#include "gamerecord.h"

#include <QDir>
#include <QFile>

GameRecord::GameRecord()
{
  recordPath = QDir::homePath() + "/.HexGame/";
  QDir *dir = new QDir();
  dir->mkpath(recordPath);
}

bool GameRecord::createFile(const QString &filename, int intCount)
{
  QFile recordFile(recordPath + filename);
  if (!recordFile.open(QIODevice::WriteOnly))
    return false;

  QDataStream out(&recordFile);
  for (int i = 0 ; i < intCount; ++i)
    out << 0;

  return true;
}

bool GameRecord::resizeFile(const QString &filename, int intCount)
{
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

int GameRecord::size(const QString &filename)
{
  QFile recordFile(recordPath + filename);
  return recordFile.size() / sizeof(int);
}

bool GameRecord::writeData(const QString &filename, int pos, int data)
{
  //TODO
  QFile recordFile(recordPath + filename);
  if (!recordFile.open(QIODevice::ReadWrite))
    return false;

  QDataStream out(&recordFile);
  recordFile.seek(pos * sizeof(int));
  out << data;

  return true;
}

int GameRecord::readData(const QString &filename, int pos)
{
  //TODO
  QFile recordFile(recordPath + filename);
  if (!recordFile.open(QIODevice::ReadOnly))
    return false;

  QDataStream in(&recordFile);
  recordFile.seek(pos * sizeof(int));
  int result;
  in >> result;
  return result;
}

bool GameRecord::writeDataArr(const QString &filename, int *dataArr, int size)
{
  //TODO
  QFile recordFile(recordPath + filename);
  if (!recordFile.open(QIODevice::ReadWrite))
    return false;

  QDataStream out(&recordFile);
  for (int i = 0; i < size; ++i)
    out << dataArr[i];

  return true;
}

bool GameRecord::readDataArr(const QString &filename, int *dataArr, int &size)
{
  //TODO
  QFile recordFile(recordPath + filename);
  if (!recordFile.open(QIODevice::ReadOnly))
    return false;

  size = size(recordFile);
  dataArr = new int[size];

  QDataStream in(&recordFile);
  for (int i = 0; i < size; ++i)
    in >> dataArr[i];

  return true;
}
