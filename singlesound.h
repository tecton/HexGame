/*
 *  singlesound.h
 */  

#ifndef SINGLESOUND_H
#define SINGLESOUND_H

#include <gst/gst.h>
#include <stdbool.h>
#include <QObject>

class QTimer;

class SingleSound : public QObject
{
Q_OBJECT
private:
    GMainLoop *loop;
    GstElement *pipeline;
    GstBus *bus;
    bool end;
    QTimer *timer;
    char* songUri;
private slots:
    void setEnd();
public:
    SingleSound(char* uri);
    void start();
    bool isEnd();
    ~SingleSound();
};

#endif
