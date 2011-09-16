/*
** Class to control the common sounds in the game.
*/

#ifndef PUBLICGAMESOUNDS_H
#define PUBLICGAMESOUNDS_H

class PublicGameSounds
{
public:
  // Try to release space, delete the sounds which has been finished
  static void tryToReleaseSpace();

  // Delete all the sounds
  static void clear();

  // The enum class of the type of common sounds
  enum GameSounds {GoodMove = 0,
                   BadMove,
                   Eliminate,
                   GetFlame,
                   GetStar,
                   UseFlame,
                   UseStar,
                   NextStage,
                   GameOver};

  // Add a sound in given type
  static void addSound(GameSounds gamesound);

  // Add a sound with the count of elimination, may be used if we create
  // different sounds for different kind of elimination
  static void addEliminate(int count);
};

#endif // PUBLICGAMESOUNDS_H
