#ifndef CONFIG_H
#define CONFIG_H

// Use QPixmap to paint or paint directly on the painter
// This should be defined only when you want to do some special effects which
// should have the information about the original looking of the widget.
//#define USE_PIXMAP

// Use translate and scale of the QPainter instead of resize the pixmap
// According to my test, you'd better not define it if you are using a meego
// tablet.
//#define USE_TRANSLATE_AND_SCALE

// Use which one to deal with sound.
// Only one of them should be chosen.
// And you may also change the .pro file.
//#define MOBILITY
//#define PHONON
//#define GSTREAMER

// On windows, we suggest you to use USE_TRANSLATE_AND_SCALE and PHONON
// On linux, we suggest you to use USE_TRANSLATE_AND_SCALE and GSTREAMER
// On meego tablet, we suggest you to use GSTREAMER
// On some devices, MOBILITY may be used, but I'm not sure whether it works
// correct or not because I don't have a such device.
// On other system, I haven't do any test, you can try it by yourself.

#endif // CONFIG_H
