#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include <QVector>
#include "mycoin.h"

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    int levelIndex;
    QVector<QVector<int>> array;
    MyCoin* coins[4][4];
    int frontNum;
    explicit PlayScene(int levelNum, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
signals:
    void PlaySceneBack();
public slots:
};

#endif // PLAYSCENE_H
