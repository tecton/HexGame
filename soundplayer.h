/*
 * soundplayer.h
 */
#include <gst/gst.h>
#include <stdbool.h>

class SoundPlayer
{
private:
    int currentSong;
    char **songPaths;
    GMainLoop *loop;
    GstElement *pipeline;
	GstBus *bus;
    enum GameSounds {GoodMove = 0,
        BadMove,
        Eliminate,
        GetFlame,
        GetStar,
        UseFlame,
        UseStar,
        NextStage,
        GameOver};
public:
    SoundPlayer(int argc, char* argv[]);
    void playSound(int number);
    void stopSound();
    ~SoundPlayer();
};
