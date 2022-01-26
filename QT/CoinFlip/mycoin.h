#ifndef MYCOIN_H
#define MYCOIN_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
#define FRONT 1
#define BACK 0



class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    int posX;
    int posY;
    bool status;
    bool disabled;
    QTimer *timer1;
    QTimer *timer2;
    int min = 1;
    int max = 8;
    MyCoin(QString btnImg, int _posX, int _posY, bool _status, bool _disabled);
    int changeStatus();
    void loadIconImg(QString img);
    void mousePressEvent(QMouseEvent* e);
signals:

public slots:
};

#endif // MYCOIN_H
