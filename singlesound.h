/*
 *  singlesound.h
 */  

#ifndef SINGLESOUND_H
#define SINGLESOUND_H

#include "config.h"

#ifdef GSTREAMER

#include <gst/gst.h>
#include <stdbool.h>
#include <QObject>
#include <ctime>
using namespace std;

/**
 * @brief A class of a single sound using gstreamer.
 */
class SingleSound : public QObject
{
Q_OBJECT
private:
    GMainLoop *loop;
    GstElement *pipeline;
    GstBus *bus;
    bool end;
    time_t createdTime;
    const char* songUri;
    int timeout;
public:
    /**
     * @brief Constructor with the path of the sound file.
     */
    SingleSound(char* uri);

    /**
     * @brief Start the sound.
     */
    void start();

    /**
     * @brief Whether the sound is end.
     */
    bool isEnd();

    /**
     * @brief Destructor.
     */
    ~SingleSound();
};

#endif
#endif
