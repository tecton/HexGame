#include "publicgamesounds.h"

#include <QSound>
#include <QList>
#include <QString>
#include <QDir>

QList <QSound *> publicGameSounds;

const static char * kPublicGameSoundsPaths[] =
{"1.wav",
 "1.wav",
 "1.wav",
 "1.wav",
 "1.wav",
 "2.wav",
 "1.wav",
 "1.wav",
 "1.wav",
 "1.wav",
 "1.wav",
 "1.wav",
 "1.wav",
 "1.wav",
 "1.wav"};

const static char * kEliminateSoundsPaths[] =
{"1.wav",
 "1.wav",
 "1.wav",
 "1.wav"};

const static int kEliminateMin = 3;
const static int kEliminateMax = 3;

void PublicGameSounds::tryToReleaseSpace()
{
//  for (QList <QSound*>::Iterator itr = publicGameSounds.begin();
//       itr != publicGameSounds.end();
//       ++itr)
//    if ((*itr)->isFinished())
//    {
//      delete *itr;
//      itr = publicGameSounds.erase(itr);
//    }
}

void PublicGameSounds::clear()
{
  QSound *sound;
  foreach (sound, publicGameSounds)
  {
    sound->stop();
    delete sound;
  }
  publicGameSounds.clear();
}

void PublicGameSounds::addSound(GameSounds gamesound)
{
  QSound *sound = new QSound(kPublicGameSoundsPaths[gamesound]);
  sound->setLoops(1);
  sound->play();
  publicGameSounds.push_back(sound);
}

void PublicGameSounds::addEliminate(int count)
{
  int index = qMax(kEliminateMin, qMin(kEliminateMax, count));
  --index;
  QSound *sound = new QSound(kEliminateSoundsPaths[index]);
  sound->setLoops(1);
  sound->play();
  publicGameSounds.push_back(sound);
}
