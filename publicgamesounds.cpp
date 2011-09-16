#include "publicgamesounds.h"

#include <QList>
#include <QString>
#include <QDir>
#include <phonon/phonon>

using namespace Phonon;

QList <MediaObject *> publicGameSounds;

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

const static char * kEliminateSoundsPaths[] =
{":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav",
 ":/commonsounds/1.wav"};

const static int kEliminateMin = 3;
const static int kEliminateMax = 3;

void PublicGameSounds::tryToReleaseSpace()
{
  for (QList <MediaObject*>::Iterator itr = publicGameSounds.begin();
       itr != publicGameSounds.end();
       ++itr)
    if ((*itr)->remainingTime() == 0)
    {
      delete *itr;
      itr = publicGameSounds.erase(itr);
    }
}

void PublicGameSounds::clear()
{
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
  MediaObject *sound = createPlayer(MusicCategory,
                                    MediaSource(kPublicGameSoundsPaths[gamesound]));
  sound->play();
  publicGameSounds.push_back(sound);
}

void PublicGameSounds::addEliminate(int count)
{
  int index = qMax(kEliminateMin, qMin(kEliminateMax, count));
  --index;
  MediaObject *sound = createPlayer(MusicCategory,
                                    MediaSource(kEliminateSoundsPaths[index]));
  sound->play();
  publicGameSounds.push_back(sound);
}
