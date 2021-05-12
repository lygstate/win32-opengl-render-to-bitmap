#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

#include <QtWin>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <QImage>
#include <QPixmap>
#include <QtWinExtras>
#include <QtOpenGL/QGLFormat>
#include <QtOpenGL/QGLPixelBuffer>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStyleOption styleOpt;
    styleOpt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
}

void Widget::createBitmap(int w, int h)
{
    width = w;
    height = h;
    memory_hdc = GetDC(nullptr);
    bitmap_hdc = CreateCompatibleDC(memory_hdc);

    BITMAPINFOHEADER binfo;
    memset(&binfo, 0, sizeof(BITMAPINFOHEADER));

    binfo.biBitCount = 24;
    binfo.biWidth = w;
    binfo.biHeight = h;
    binfo.biSize = sizeof(binfo);
    binfo.biPlanes = 1;
    binfo.biCompression = BI_RGB;

    bitmap = CreateDIBSection(bitmap_hdc, reinterpret_cast<BITMAPINFO*>(&binfo), DIB_RGB_COLORS, nullptr, nullptr, 0);
    SelectObject(bitmap_hdc, bitmap);
}

DWORD Widget::init()
{
    createBitmap(200, 200);

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;

    pfd.dwFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;

    int iPixelFormat = ChoosePixelFormat(bitmap_hdc, &pfd);
    if (iPixelFormat != 0)
    {
        PIXELFORMATDESCRIPTOR bestMatch_pfd;
        if (DescribePixelFormat(bitmap_hdc, iPixelFormat, sizeof(pfd), &bestMatch_pfd) == FALSE)
        {
            ERROR_LOCKED;
            return GetLastError();
        }

        if (bestMatch_pfd.cDepthBits < pfd.cDepthBits)
        {
            return ERROR_INVALID_PARAMETER;
        }

        if (SetPixelFormat(bitmap_hdc, iPixelFormat, &pfd) == FALSE)
        {
            return GetLastError();
        }
    }
    else
    {
        return GetLastError();
    }

    bitmap_hrc = wglCreateContext(bitmap_hdc);
    wglMakeCurrent(bitmap_hdc, bitmap_hrc);
    return ERROR_SUCCESS;
}

int count = 0;

void Widget::on_pushButton_clicked()
{
    ++count;
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    int index = 5 - (count % 5);
    glColor3f(0.2f * index, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.1f* index, -0.1f * index);
    glVertex2f(0.0f, 0.1f * index);
    glVertex2f(0.1f * index, -0.1f * index);
    glEnd();
    glFlush();

    auto image = QtWin::imageFromHBITMAP(bitmap_hdc, bitmap, width, height);
    ui->label->setPixmap(QPixmap::fromImage(image));
}
