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

class SingleSound : public QObject
{
Q_OBJECT
private:
    GMainLoop *loop;
    GstElement *pipeline;
    GstBus *bus;
    bool end;
    time_t createdTime;
    char* songUri;
public:
    SingleSound(char* uri);
    void start();
    bool isEnd();
    ~SingleSound();
};

#endif
#endif
