/*
** Class to control the common sounds in the game.
*/

#ifndef PUBLICGAMESOUNDS_H
#define PUBLICGAMESOUNDS_H

class PublicGameSounds
{
public:
  static void tryToReleaseSpace();
  static void clear();

  enum GameSounds {GoodMove = 0,
                   BadMove,
                   Eliminate,
                   GetFlame,
                   GetStar,
                   UseFlame,
                   UseStar,
                   NextStage,
                   GameOver};

  static void addSound(GameSounds gamesound);
  static void addEliminate(int count);
};

#endif // PUBLICGAMESOUNDS_H
