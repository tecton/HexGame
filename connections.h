/*
** A class to store the infomation of connections.
*/

#ifndef CONNECTIONS_H
#define CONNECTIONS_H

// File most include
#include <QVector>

class Connections
{
public:
  /**
   * @brief Constructor with the number of the balls.
   */
  Connections(int theBallCount) :
      ballCount(theBallCount),
      connectionsOfIndex(
          new QVector<QVector<int> *>[ballCount])
  {
    for (int i = 0;i < ballCount;++i)
      connectionsOfIndex[i].fill(NULL, 10);
  }

  /**
   * @brief Constructor with another Connection.
   */
  Connections(const Connections& another) :
      ballCount(another.ballCount),
      connectionsOfIndex(
          new QVector<QVector<int> *>[ballCount])
  {
    // Copy the information
    for (int i = 0;i < another.connections.size();++i)
      connections.push_back(
          new QVector<int>(*another.connections.at(i)));

    for (int i = 0;i < ballCount;++i)
    {
      connectionsOfIndex[i].reserve(
          another.connectionsOfIndex[i].size());
      for (int j = 0;
           j < another.connectionsOfIndex[i].size();
           ++j)
      {
        if (another.connectionsOfIndex[i].at(j))
          connectionsOfIndex[i].push_back(
              new QVector<int>(
                  *another.connectionsOfIndex[i].at(j)));
        else
          connectionsOfIndex[i].push_back(NULL);
      }
    }
  }

  /**
   * @brief Destructor.
   */
  ~Connections()
  {
    for (int i = 0;i < connections.size();++i)
      delete connections[i];
    delete [] connectionsOfIndex;
  }

  /**
   * @brief Whether a ball is in a chain.
   *
   * @param index The index of the ball to check.
   */
  bool isInAChain(int index)
  {
    for (int i = 0;i < 10;++i)
      if (i != 3 && connectionsOfIndex[index][i])
        return true;
    return false;
  }

  /**
   * @brief Whether a ball is the center of a chain.
   *
   * @param index The index of the ball to check.
   */
  bool isCenterOfAChain(int index)
  {
    return connectionsOfIndex[index][3];
  }

  /**
   * @brief Number of the balls.
   */
  int ballCount;

  QVector<QVector<int> *> *connectionsOfIndex;

  /**
   * @brief All of the connections.
   */
  QVector<QVector<int> *> connections;
};

#endif // CONNECTIONS_H
