#include <QApplication>
#include <QThread>
#include <process.h>

#include <gldisplayoffscreen.h>

#include "widget.h"

void OpenGLDrawThread(void *param)
{
    Widget *w = (Widget*)param;
    auto onUpdateImage = [=](const QImage &image) {
        QMetaObject::invokeMethod(w, "updateImage", Qt::QueuedConnection, Q_ARG(const QImage &, image));
    };
    GlDisplayOffscreen offscreen(500, 500, onUpdateImage);
    for (;;) {
        QThread::msleep(100);
        offscreen.updateOpenGL();
    }
}

uintptr_t pThread;
void OnAppRun(Widget *w)
{
    pThread = ::_beginthread(OpenGLDrawThread, 0, w);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.show();
    OnAppRun(&w);
    return a.exec();
}
