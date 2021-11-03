#include "mainscene.h"
#include "ui_mainscene.h"
#include "mypushbutton.h"
#include <QDebug>
#include <QTimer>
#include <QSound>
#include <QPainter>

MainScene::MainScene(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainScene)
{
    ui->setupUi(this);
    //配置主场景
    setFixedSize(320, 588);
    setWindowIcon(QIcon(":/res/Coin0001.png"));
    setWindowTitle("翻金币游戏");


    ChooseScene = new ChooseLevelScene();

    connect(ChooseScene, &ChooseLevelScene::chooseSceneBack, this, [=](){
        this->setGeometry(ChooseScene->geometry());
        ChooseScene->hide();
        this->show();
    });

    connect(ui->actionQuit, &QAction::triggered, this, [=](){
        this->close();
    });

    //开始按钮
    MyPushButton* startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move((this->width() - startBtn->width()) * 0.5, this->height() * 0.7);
    QSound *StartSound = new QSound(":/res/TapButtonSound.wav", this);

    connect(startBtn, &MyPushButton::clicked, this, [=](){
           StartSound->play();
           startBtn->zoom1();
           startBtn->zoom2();
           ChooseScene->setGeometry(this->geometry());
           QTimer::singleShot(500, this, [=](){
               this->hide();
               ChooseScene->show();
           });
    });
}

void MainScene::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);
    painter.drawPixmap(10, 30, pix);
}

MainScene::~MainScene()
{
    delete ui;
}
