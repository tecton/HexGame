/*
 * soundplayer.cpp
 */

#include "config.h"

#ifdef GSTREAMER
#include "soundplayer.h"
#include <stdio.h>
#include <QList>

QList <SingleSound *> gameSounds;

SoundPlayer::SoundPlayer()
{
    songPaths = new char*[9];
    // path should be corresponding to sound files.
    songPaths[0] = "file:///media/shared/gameover.wav";
    songPaths[1] = "file:///media/shared/nextstage.wav";
    songPaths[2] = "file:///media/shared/getflame.wav";
    songPaths[3] = "file:///media/shared/getstar.wav";
    songPaths[4] = "file:///media/shared/useflame.wav";
    songPaths[5] = "file:///media/shared/usestar.wav";
    songPaths[6] = "file:///media/shared/eliminate.wav";
    songPaths[7] = "file:///media/shared/badmove.wav";
}

void SoundPlayer::tryToReleaseSpace()
{
    if (gameSounds.size() < 5)
    {
        printf("no release.\n");
        return;
    }
    printf("release.\n");
    SingleSound *sound;
    foreach (sound, gameSounds)
    {
        if (sound->isEnd())
        {
            delete sound;
            gameSounds.removeOne(sound);
        }
    }
}

void SoundPlayer::clear()
{
    SingleSound *sound;
    foreach (sound, gameSounds)
    {
        delete sound;
        gameSounds.removeOne(sound);
    }
    gameSounds.clear();
}

void SoundPlayer::addSound(int number)
{
    tryToReleaseSpace();
    SingleSound *sound = new SingleSound(songPaths[number]);
    sound->start();
    gameSounds.push_back(sound);
}

SoundPlayer::~SoundPlayer()
{
    clear();
    delete []songPaths;
}
#endif
