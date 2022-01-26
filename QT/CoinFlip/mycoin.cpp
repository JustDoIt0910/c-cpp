#include "mycoin.h"
#include <QDebug>

MyCoin::MyCoin(QString btnImg, int _posX, int _posY, bool _status, bool _disabled) : posX(_posX), posY(_posY), status(_status), disabled(_disabled)
{
    QPixmap pix(btnImg);
    setFixedSize(pix.width(), pix.height());
    setIcon(pix);
    setIconSize(QSize(this->width(), this->height()));
    setStyleSheet("QPushButton{border:0px;}");

    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    connect(timer1, &QTimer::timeout, this, [=](){
        QString path = QString(":/res/Coin000%1.png").arg(this->min++);
        loadIconImg(path);
        if(this->min > this->max)
        {
            this->min = 1;
            timer1->stop();
        }
    });
    connect(timer2, &QTimer::timeout, this, [=](){
        QString path = QString(":/res/Coin000%1.png").arg(this->max--);
        loadIconImg(path);
        if(this->max < this->min)
        {
            this->max = 8;
            timer2->stop();
        }
    });
}

int MyCoin::changeStatus()
{
    if(status == FRONT)
    {
        status = BACK;
        timer1->start(30);
    }
    else
    {
        status = FRONT;
        timer2->start(30);
    }
    return status;
}

void MyCoin::loadIconImg(QString img)
{
    QPixmap pix;
    bool ret = pix.load(img);
    if(!ret)
    {
        qDebug() << "图片加载失败";
        return;
    }
    this->setFixedSize(pix.width(), pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(), pix.height()));
}

void MyCoin::mousePressEvent(QMouseEvent* e)
{
    if(disabled)
        return;
    QPushButton::mousePressEvent(e);
}
