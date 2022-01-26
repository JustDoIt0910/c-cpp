#include "chooselevelscene.h"
#include "playscene.h"
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include <QMenuBar>
#include <QPainter>
#include <QSound>


ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(320, 588);
    setWindowIcon(QIcon(":/res/Coin0001.png"));
    setWindowTitle("选择关卡");

    QMenuBar *bar = menuBar();
    setMenuBar(bar);
    QMenu *menu = bar->addMenu("开始");
    QAction *actionQuit = menu->addAction("退出");

    connect(actionQuit, &QAction::triggered, this, [=](){
        this->close();
    });

    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected");
    QSound *backSound = new QSound(":/res/BackButtonSound.wav", this);
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());
    connect(backBtn, &MyPushButton::clicked, this, [=](){
        backSound->play();
        QTimer::singleShot(500, this, [=](){
            emit this->chooseSceneBack();
        });
    });

    for(int i = 0; i < 20; i++)
    {
        MyPushButton *levelBtn = new MyPushButton(":res/LevelIcon.png");
        levelBtn->setParent(this);
        levelBtn->move((i % 4) * 70 + 25, (i / 4) *70 + 135);
        QLabel *label = new QLabel(this);
        label->setFixedSize(levelBtn->width(), levelBtn->height());
        label->setText(QString::number(i + 1));
        label->move((i % 4) * 70 + 25, (i / 4) *70 + 135);
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        label->setAttribute(Qt::WA_TransparentForMouseEvents);

        connect(levelBtn, &MyPushButton::clicked, this, [=](){
            this->hide();
            play = new PlayScene(i + 1);
            play->setGeometry(this->geometry());
            play->show();

            connect(play, &PlayScene::PlaySceneBack, this, [=](){
                this->setGeometry(play->geometry());
                this->show();
                delete play;
                play = nullptr;
            });
        });
    }
}

void ChooseLevelScene::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);
    painter.drawPixmap(10, 30, pix);
}
