#ifndef GLDISPLAYOFFSCREEN_H
#define GLDISPLAYOFFSCREEN_H

#include <Windows.h>
#include <functional>
#include <QImage>

class GlDisplayOffscreen
{
public:
    GlDisplayOffscreen(
            int w, int h,
            std::function<void(const QImage &image)> _onUpdateImage);
    int width, height;
    HGLRC bitmap_hrc;
    HBITMAP bitmap;
    HDC bitmap_hdc;
    HDC memory_hdc;
    int count;

    void createBitmap(int w, int h);
    DWORD init();
    void updateOpenGL();

    std::function<void(const QImage &image)> onUpdateImage;
};

#endif // GLDISPLAYOFFSCREEN_H
