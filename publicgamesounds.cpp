#include "publicgamesounds.h"

#include <QList>
#include <QString>
#include <QDir>

#include "config.h"

#ifdef MOBILITY
#include <QMediaPlayer>
QList <QMediaPlayer *> publicGameSounds;
#endif

#ifdef PHONON
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
#ifdef PHONON
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

#ifdef MOBILITY
  if (publicGameSounds.size() < 10)
    return;

  // Delete the finished sounds
  QMediaPlayer *mediaObject;
  foreach (mediaObject, publicGameSounds)
  {
    if (mediaObject->mediaStatus() < 2 || mediaObject->mediaStatus() > 6)
    {
      delete mediaObject;
      publicGameSounds.removeOne(mediaObject);
    }
  }
#endif
}

void PublicGameSounds::clear()
{
#ifdef PHONON
  // Delete all the sounds
  MediaObject *sound;
  foreach (sound, publicGameSounds)
  {
    sound->stop();
    delete sound;
  }
  publicGameSounds.clear();
#endif

#ifdef MOBILITY
  // Delete all the sounds
  QMediaPlayer *sound;
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
#ifdef PHONON
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

#ifdef MOBILITY
  tryToReleaseSpace();

  // Create the sound
  QMediaPlayer *sound = new QMediaPlayer;
  sound->setMedia(QUrl::fromLocalFile(kPublicGameSoundsPaths[gamesound]));
  sound->setVolume(50);

  // Play the sound
  sound->play();

  // Record the sound
  publicGameSounds.push_back(sound);
#endif
}

void PublicGameSounds::addEliminate(int count)
{
#ifdef PHONON
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

#ifdef MOBILITY
  tryToReleaseSpace();
  int index = qMax(kEliminateMin, qMin(kEliminateMax, count));
  --index;

  // Create the sound
  QMediaPlayer *sound = new QMediaPlayer;
  sound->setMedia(QUrl::fromLocalFile(kEliminateSoundsPaths[index]));
  sound->setVolume(50);

  // Play the sound
  sound->play();

  // Record the sound
  publicGameSounds.push_back(sound);
#endif
}
