#include "playscene.h"
#include "mypushbutton.h"
#include "mycoin.h"
#include "dataconfig.h"
#include <QPropertyAnimation>
#include <QSound>
#include <QString>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QFont>
#include <QDebug>


PlayScene::PlayScene(int levelNum, QWidget *parent) : QMainWindow(parent), levelIndex(levelNum), frontNum(0)
{
    setFixedSize(320, 588);
    setWindowIcon(QIcon(":/res/Coin0001.png"));
    QString title = QString("第%1关").arg(this->levelIndex);
    setWindowTitle(title);
    QString s = QString("选择了第 %1 关").arg(this->levelIndex);
    qDebug() << s;

    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected");
    QSound *backSound = new QSound(":/res/BackButtonSound.wav", this);
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());
    connect(backBtn, &MyPushButton::clicked, this, [=](){
        backSound->play();
        QTimer::singleShot(500, this, [=](){
            emit this->PlaySceneBack();
        });
    });
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QLabel *label = new QLabel(this);
    label->setGeometry(30, this->height() - 50, 150, 50);
    label->setFont(font);
    QString text = QString("Level %1").arg(this->levelIndex);
    label->setText(text);


    QLabel *winLabel = new QLabel(this);
    QPixmap pix(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0, 0, pix.width(), pix.height());
    winLabel->setPixmap(pix);
    winLabel->move((this->width() - winLabel->width()) * 0.5, -winLabel->height());

    QSound *flipSound = new QSound(":/res/ConFlipSound.wav", this);
    QSound *winSound = new QSound(":/res/LevelWinSound.wav", this);

    dataConfig config;
    array = config.mData[this->levelIndex];
    int dx[] = {0, -1, 0, 1};
    int dy[] = {-1, 0, 1, 0};

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
        {
            QPixmap pix;
            pix.load(":/res/BoardNode.png");
            QLabel *label = new QLabel(this);
            label->setGeometry(0, 0, pix.width(), pix.height());
            label->setPixmap(pix);
            label->move(i * 50 + 57, j * 50 + 200);

            QString path = array[i][j] == 1 ? ":/res/Coin0001.png" : ":/res/Coin0008.png";
            frontNum = array[i][j] == 1 ? frontNum + 1 : frontNum;

            MyCoin *coin = new MyCoin(path, i, j, array[i][j], false);
            coin->setParent(this);
            coin->move(i * 50 + 59, j * 50 + 204);
            coins[i][j] = coin;

            connect(coin, &MyCoin::clicked, this, [=](){
                flipSound->play();
                frontNum += coin->changeStatus() == 1 ? 1 : -1;
                QTimer::singleShot(150, [=](){
                    for(int k = 0; k < 4; k++)
                    {
                        int nx = coin->posX + dx[k];
                        int ny = coin->posY + dy[k];
                        if(nx >= 0 && nx < 4 && ny >= 0 && ny < 4)
                        {
                            frontNum += coins[nx][ny]->changeStatus() == 1 ? 1 : -1;
                        }
                    }
                    if(frontNum == 16)
                    {
                        for(int cx = 0; cx < 4; cx++)
                            for(int cy = 0; cy < 4; cy++)
                                coins[cx][cy]->disabled = true;
                        qDebug() << "Win!";
                        winSound->play();
                        QPropertyAnimation *animation = new QPropertyAnimation(winLabel, "geometry");
                        animation->setDuration(1000);
                        animation->setStartValue(QRect(winLabel->x(), winLabel->y(), winLabel->width(), winLabel->height()));
                        animation->setEndValue(QRect(winLabel->x(), winLabel->y() + 114, winLabel->width(), winLabel->height()));
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        animation->start();
                    }
                });
            });
        }
}

void PlayScene::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);
    painter.drawPixmap(10, 30, pix);
}
