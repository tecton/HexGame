/*
 * soundplayer.h
 */
#include "config.h"
#include <QObject>
#ifdef GSTREAMER
#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include "singlesound.h"

/**
 * @brief A class to play sounds.
 */
class SoundPlayer
{
private:
    int currentSong;
    char **songPaths;
public:
    enum GameSounds {GameOver = 0,
        NextStage,
        GetFlame,
        GetStar,
        UseFlame,
        UseStar,
        Eliminate,
        BadMove};
    SoundPlayer();
    void tryToReleaseSpace();
    void clear();
    void addSound(int number);
    ~SoundPlayer();
};

#endif
#endif
