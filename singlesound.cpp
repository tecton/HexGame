#include "config.h"
#ifdef GSTREAMER
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

void SingleSound::cacheSound(const char* uri)
{
    // init gstreamer
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    GstElement *pipeline = gst_element_factory_make("playbin", "player");
    GstBus *bus = NULL;
    g_object_set(G_OBJECT(pipeline), "uri", uri, NULL);
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, bus_call, loop);
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
    g_main_loop_run(loop);
    g_main_loop_quit(loop);
    // set state
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
    // unref objects
    gst_object_unref(bus);
    gst_object_unref(GST_OBJECT(pipeline));
}

SingleSound::SingleSound(const char* uri, const int outTime) : songUri(uri), timeout(outTime)
{
    // init gstreamer
    loop = g_main_loop_new(NULL, FALSE);
    pipeline = gst_element_factory_make("playbin", "player");
    bus = NULL;
    end = false;
    createdTime = time(NULL);
}

void SingleSound::start()
{
    //printf("%s\n", songUri);
    g_object_set(G_OBJECT(pipeline), "uri", songUri, NULL);
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, bus_call, loop);

    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
    g_main_loop_run(loop);
    createdTime = time(NULL);
}

bool SingleSound::isEnd()
{
    time_t currentTime = time(NULL);
    return (currentTime - createdTime) > timeout;
}

SingleSound::~SingleSound()
{
    g_main_loop_quit(loop);
    // set state
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
    // unref objects
    gst_object_unref(bus);
    gst_object_unref(GST_OBJECT(pipeline));
}

#endif
