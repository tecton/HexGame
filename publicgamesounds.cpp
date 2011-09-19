#include "publicgamesounds.h"

#include <QList>
#include <QString>
#include <QDir>

//#define USE_SOUND

#ifdef USE_SOUND
#include <phonon/phonon>

// Set the name space
using namespace Phonon;
// The list to store the sounds
QList <MediaObject *> publicGameSounds;
#endif

// The path of the sounds
const static char * kPublicGameSoundsPaths[] =
{":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/2.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav"};

// The path of the sounds
const static char * kEliminateSoundsPaths[] =
{":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav"};

// Some value may be used
const static int kEliminateMin = 3;
const static int kEliminateMax = 3;

void PublicGameSounds::tryToReleaseSpace()
{
#ifdef USE_SOUND
  if (publicGameSounds.size() < 10)
    return;

  // Delete the finished sounds
  MediaObject *mediaObject;
  foreach (mediaObject, publicGameSounds)
  {
    if (mediaObject->remainingTime() == 0)
    {
      delete mediaObject;
      publicGameSounds.removeOne(mediaObject);
    }
  }
#endif
}

void PublicGameSounds::clear()
{
#ifdef USE_SOUND
  // Delete all the sounds
  MediaObject *sound;
  foreach (sound, publicGameSounds)
  {
    sound->stop();
    delete sound;
  }
  publicGameSounds.clear();
#endif
}

void PublicGameSounds::addSound(GameSounds gamesound)
{
#ifdef USE_SOUND
  tryToReleaseSpace();

  // Create the sound
  MediaObject *sound =
      createPlayer(MusicCategory,
                   MediaSource(kPublicGameSoundsPaths[gamesound]));

  // Play the sound
  sound->play();

  // Record the sound
  publicGameSounds.push_back(sound);
#endif
}

void PublicGameSounds::addEliminate(int count)
{
#ifdef USE_SOUND
  tryToReleaseSpace();
  int index = qMax(kEliminateMin, qMin(kEliminateMax, count));
  --index;

  // Create the sound
  MediaObject *sound =
      createPlayer(MusicCategory,
                   MediaSource(kEliminateSoundsPaths[index]));

  // Play the sound
  sound->play();

  // Record the sound
  publicGameSounds.push_back(sound);
#endif
}
