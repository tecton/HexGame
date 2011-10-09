/*
 * soundplayer.h
 */
#include "config.h"

#ifdef GSTREAMER
#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <gst/gst.h>
#include <stdbool.h>

/**
 * @brief A class to play sounds.
 */
class SoundPlayer
{
private:
    int currentSong;
    char **songPaths;
    // needed by gstreamer
    GMainLoop *loop;
    GstElement *pipeline;
	GstBus *bus;
public:
    enum GameSounds {GoodMove = 0,
        BadMove,
        Eliminate,
        GetFlame,
        GetStar,
        UseFlame,
        UseStar,
        NextStage,
        GameOver};
    SoundPlayer();
    void playSound(int number);
    void stopSound();
    ~SoundPlayer();
};

#endif
#endif
