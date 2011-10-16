#include "publicgamesounds.h"

#include <QList>
#include <QVector>
#include <QString>
#include <QDir>

#include "config.h"

#ifdef GSTREAMER
#include "singlesound.h"
QList <SingleSound *> soundsPlaying;
#endif

#ifdef MOBILITY
#include <QMediaPlayer>
QList <QMediaPlayer *> soundsPlaying;
#endif

#ifdef PHONON
#include <phonon/phonon>
// Set the name space
using namespace Phonon;
// The list to store the sounds
QList <MediaObject *> soundsPlaying;
QVector <MediaSource> soundSources;
#endif

const static int kSoundCount = 8;

// The path of the sounds
const static char * kSoundPaths[] =
{"C:/sounds/badmove.wav",
 "C:/sounds/eliminate.wav",
 "C:/sounds/getflame.wav",
 "C:/sounds/getstar.wav",
 "C:/sounds/useflame.wav",
 "C:/sounds/usestar.wav",
 "C:/sounds/nextstage.wav",
 "C:/sounds/gameover.wav",
 "C:/sounds/1.wav",
 "C:/sounds/1.wav",
 "C:/sounds/1.wav",
 "C:/sounds/1.wav",
 "C:/sounds/1.wav",
 "C:/sounds/1.wav",
 "C:/sounds/1.wav"};

// The path of the sounds
const static char * kEliminateSoundsPaths[] =
{"C:/sounds/1.wav",
 "C:/sounds/1.wav",
 "C:/sounds/1.wav",
 "C:/sounds/1.wav"};

// Some value may be used
const static int kEliminateMin = 3;
const static int kEliminateMax = 3;

void PublicGameSounds::init()
{
#ifdef GSTREAMER
  gst_init(NULL,NULL);
#endif

#ifdef PHONON
  for (int i = 0;i < kSoundCount;++i)
    soundSources.push_back(MediaSource(kSoundPaths[i]));
#endif

#ifdef MOBILITY
#endif
}

void PublicGameSounds::tryToReleaseSpace()
{
#ifdef GSTREAMER
  if (soundsPlaying.size() < 5)
    return;

  SingleSound *sound;
  foreach (sound, soundsPlaying)
  {
    if (sound->isEnd())
    {
      delete sound;
      soundsPlaying.removeOne(sound);
    }
  }
#endif

#ifdef PHONON
  if (soundsPlaying.size() < 5)
    return;

  // Delete the finished sounds
  MediaObject *mediaObject;
  foreach (mediaObject, soundsPlaying)
  {
    if (mediaObject->remainingTime() == 0)
    {
      delete mediaObject;
      soundsPlaying.removeOne(mediaObject);
    }
  }
#endif

#ifdef MOBILITY
  if (soundsPlaying.size() < 10)
    return;

  // Delete the finished sounds
  QMediaPlayer *mediaObject;
  foreach (mediaObject, soundsPlaying)
  {
    if (mediaObject->mediaStatus() < 2 || mediaObject->mediaStatus() > 6)
    {
      delete mediaObject;
      soundsPlaying.removeOne(mediaObject);
    }
  }
#endif
}

void PublicGameSounds::clear()
{
#ifdef GSTREAMER
  SingleSound *sound;
  foreach (sound, soundsPlaying)
    delete sound;
  soundsPlaying.clear();
#endif
#ifdef PHONON
  // Delete all the sounds
  MediaObject *sound;
  foreach (sound, soundsPlaying)
  {
    sound->stop();
    delete sound;
  }
  soundsPlaying.clear();
#endif

#ifdef MOBILITY
  // Delete all the sounds
  QMediaPlayer *sound;
  foreach (sound, soundsPlaying)
  {
    sound->stop();

    delete sound;
  }
  soundsPlaying.clear();
#endif
}

void PublicGameSounds::addSound(GameSounds gamesound)
{
#ifdef GSTREAMER
  if (gamesound != -1)
  {
    tryToReleaseSpace();
    SingleSound *sound = new SingleSound(kSoundPaths[gamesound]);
    sound->start();
    soundsPlaying.push_back(sound);
  }
#endif

#ifdef PHONON
  tryToReleaseSpace();


  if (gamesound != -1)
  {
    // Create the sound
    MediaObject *sound =
        createPlayer(MusicCategory,
                     soundSources[gamesound]);

    // Play the sound
    sound->play();

    // Record the sound
    soundsPlaying.push_back(sound);
  }
#endif

#ifdef MOBILITY
  tryToReleaseSpace();

  // Create the sound
  QMediaPlayer *sound = new QMediaPlayer;
  sound->setMedia(QUrl::fromLocalFile(kSoundPaths[gamesound]));
  sound->setVolume(50);

  // Play the sound
  sound->play();

  // Record the sound
  soundsPlaying.push_back(sound);
#endif
}

void PublicGameSounds::addEliminate(int count)
{
  PublicGameSounds::addSound(PublicGameSounds::Eliminate);
}
