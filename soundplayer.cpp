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
            //g_message("End-of-stream");
            printf("end : %d\n", GST_MESSAGE_TYPE(msg));
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
            printf("default : %d\n", GST_MESSAGE_TYPE(msg));
            g_main_loop_quit((GMainLoop*)user_data);
            break;
	}
    
	return true;
}

SoundPlayer::SoundPlayer(int argc, char* argv[])
{
    currentSong = -1;
    songPaths = new char*[9];
    songPaths[0] = "file:///media/shared/2.wav";
    songPaths[1] = "file:///media/shared/1.wav";
    songPaths[2] = "file:///media/shared/2.wav";
    songPaths[3] = "file:///media/shared/1.wav";
    songPaths[4] = "file:///media/shared/2.wav";
    songPaths[5] = "file:///media/shared/1.wav";
    songPaths[6] = "file:///media/shared/2.wav";
    songPaths[7] = "file:///media/shared/1.wav";
    songPaths[8] = "file:///media/shared/2.wav";
//    gst_init(&argc, &argv);
//    loop = g_main_loop_new(NULL, FALSE);
//    pipeline = gst_element_factory_make("playbin", "player");
    bus = NULL;
}

void SoundPlayer::playSound(int number)
{
    if (currentSong != -1) {
        stopSound();
    }
    currentSong = number;
    gst_init(NULL, NULL);
    loop = g_main_loop_new(NULL, FALSE);
    pipeline = gst_element_factory_make("playbin", "player");
    g_object_set(G_OBJECT(pipeline), "uri", songPaths[number], NULL);
//    if (!bus) {
//        bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
//        gst_bus_add_watch(bus, bus_call, loop);
//        gst_object_unref(bus);
//    }
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, bus_call, loop);
    gst_object_unref(bus);
    
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
//    printf("2\n");
    printf("%d\n", currentSong);
    g_main_loop_run(loop);
}

void SoundPlayer::stopSound()
{
    //g_main_loop_quit(loop);
    //gst_message_new_eos(GST_ELEMENT(pipeline));
    
    gst_bus_post(bus, gst_message_new_eos(GST_OBJECT(pipeline)));
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    currentSong = -1;
    printf("%d\n", currentSong);
}

SoundPlayer::~SoundPlayer()
{
    printf("end\n");
    if (currentSong != -1)
    {
        stopSound();
//        gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
//        gst_object_unref(GST_OBJECT(pipeline));
    }
    delete []songPaths;
}
#endif
