/*
 * soundplayer.h
 */
#include "config.h"
#include <QObject>
#ifdef GSTREAMER
#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <gst/gst.h>
#include <stdbool.h>

class QTimer;

/**
 * @brief A class to play sounds.
 */
class SoundPlayer : public QObject
{
Q_OBJECT
private:
    int currentSong;
    char **songPaths;
    // needed by gstreamer
    GMainLoop *loop;
    GstElement *pipeline;
	GstBus *bus;
	QTimer *timer;
private slots:
    void setSong();
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
    void playSound(int number);
    void stopSound();
    ~SoundPlayer();
};

#endif
#endif
