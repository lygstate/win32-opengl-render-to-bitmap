#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

#include <QtWin>
#include <QImage>
#include <QPixmap>
#include <QtWinExtras>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updateImage(const QImage &image)
{
    ui->label->setPixmap(QPixmap::fromImage(image));
}
