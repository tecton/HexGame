/*
 * soundplayer.cpp
 */

#ifdef GSTREAMER
#include "soundplayer.h"
#include <stdio.h>

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

SoundPlayer::SoundPlayer()
{
    currentSong = -1;
    songPaths = new char*[9];
    // path should be corresponding to sound files.
    songPaths[0] = "file:///media/shared/goodmove.wav";
    songPaths[1] = "file:///media/shared/badmove.wav";
    songPaths[2] = "file:///media/shared/eliminate.wav";
    songPaths[3] = "file:///media/shared/getflame.wav";
    songPaths[4] = "file:///media/shared/getstar.wav";
    songPaths[5] = "file:///media/shared/useflame.wav";
    songPaths[6] = "file:///media/shared/usestar.wav";
    songPaths[7] = "file:///media/shared/nextstage.wav";
    songPaths[8] = "file:///media/shared/gameover.wav";
    // init gstreamer
    gst_init(NULL, NULL);
    loop = g_main_loop_new(NULL, FALSE);
    pipeline = gst_element_factory_make("playbin", "player");
    bus = NULL;
}

void SoundPlayer::playSound(int number)
{
    if (currentSong != -1) {
        // stop the current playing sound.
        stopSound();
    }
    currentSong = number;
    g_object_set(G_OBJECT(pipeline), "uri", songPaths[number], NULL);
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, bus_call, loop);
    
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
    g_main_loop_run(loop);
}

void SoundPlayer::stopSound()
{
    // send a message
    gst_bus_post(bus, gst_message_new_eos(GST_OBJECT(pipeline)));
    // set state
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
    currentSong = -1;
}

SoundPlayer::~SoundPlayer()
{
//    printf("end\n");
    if (currentSong != -1)
    {
        // still playing...
        stopSound();
        // unref objects
        gst_object_unref(bus);
        gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(pipeline));
    }
    delete []songPaths;
}
#endif