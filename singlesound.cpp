/*
 *  singlesound.cpp
 */  


#include <QTimer>
#include <stdio.h>
#include "singlesound.h"

gboolean bus_call(GstBus *bus, GstMessage *msg, void *user_data)
{
	switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_EOS: {
            g_main_loop_quit((GMainLoop*)user_data);
            break;
        }
        case GST_MESSAGE_ERROR: {
            GError *err;
            gst_message_parse_error(msg, &err, NULL);
            g_error("%s", err->message);
            g_error_free(err);
            
            g_main_loop_quit((GMainLoop*)user_data);
            break;
        }
        default:
            g_main_loop_quit((GMainLoop*)user_data);
            break;
	}
	return true;
}

SingleSound::SingleSound(char* uri)
{
    // init gstreamer
    gst_init(NULL, NULL);
    loop = g_main_loop_new(NULL, FALSE);
    pipeline = gst_element_factory_make("playbin", "player");
    bus = NULL;
    end = false;
    songUri = uri;
    timer = new QTimer();
    timer->setInterval(1500);
    connect(timer, SIGNAL(timeout()), this, SLOT(setEnd()));
    printf("uri: %s\n", songUri);
}

void SingleSound::start()
{
    printf("%s\n", songUri);
    g_object_set(G_OBJECT(pipeline), "uri", songUri, NULL);
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, bus_call, loop);
    
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
    g_main_loop_run(loop);
    timer->start();
}

bool SingleSound::isEnd()
{
    return end;
}

void SingleSound::setEnd()
{
    end = true;
    timer->stop();
}

SingleSound::~SingleSound()
{
    g_main_loop_quit(loop);
    // set state
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
    // unref objects
    gst_object_unref(bus);
    gst_object_unref(GST_OBJECT(pipeline));
    // release timer
    timer->stop();
    delete timer;
}
