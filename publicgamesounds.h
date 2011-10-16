#ifndef PUBLICGAMESOUNDS_H
#define PUBLICGAMESOUNDS_H

/**
 * @brief A class to control the common sounds in the game.
 */
class PublicGameSounds
{
public:
  /**
   * @brief Init the sounds, load them to memory.
   */
  static void init();

  /**
   * @brief Try to release space, delete the sounds which has been finished.
   */
  static void tryToReleaseSpace();

  /**
   * @brief Delete all the sounds.
   */
  static void clear();

  /**
   * @brief Type of common sounds.
   */
  enum GameSounds {GoodMove = -1,
                   BadMove = 0,
                   Eliminate,
                   GetFlame,
                   GetStar,
                   UseFlame,
                   UseStar,
                   NextStage,
                   GameOver};

  /**
   * @brief Add a sound in given type.
   */
  static void addSound(GameSounds gamesound);

  /**
   * @brief Add a sound with the count of elimination, may be used if we
   *  create different sounds for different kind of elimination.
   */
  static void addEliminate(int count);
};

#endif // PUBLICGAMESOUNDS_H
