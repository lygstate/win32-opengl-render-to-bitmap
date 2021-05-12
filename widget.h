#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <Windows.h>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void on_pushButton_clicked();

private:
    void paintEvent(QPaintEvent * event);

private:
    Ui::Widget *ui;

    int width, height;
    HGLRC bitmap_hrc;
    HBITMAP bitmap;
    HDC bitmap_hdc;
    HDC memory_hdc;

    void createBitmap(int w, int h);
    DWORD init();
};

#endif // WIDGET_H
