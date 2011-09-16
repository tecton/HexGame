#include "publicgamesounds.h"

#include <QList>
#include <QString>
#include <QDir>
#include <phonon/phonon>

// Set the name space
using namespace Phonon;

// The list to store the sounds
QList <MediaObject *> publicGameSounds;

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
}

void PublicGameSounds::clear()
{
  // Delete all the sounds
  MediaObject *sound;
  foreach (sound, publicGameSounds)
  {
    sound->stop();
    delete sound;
  }
  publicGameSounds.clear();
}

void PublicGameSounds::addSound(GameSounds gamesound)
{
  tryToReleaseSpace();

  // Create the sound
  MediaObject *sound =
      createPlayer(MusicCategory,
                   MediaSource(kPublicGameSoundsPaths[gamesound]));

  // Play the sound
  sound->play();

  // Record the sound
  publicGameSounds.push_back(sound);
}

void PublicGameSounds::addEliminate(int count)
{
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
}
