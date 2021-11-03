#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include "chooselevelscene.h"

namespace Ui {
class MainScene;
}

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    ChooseLevelScene * ChooseScene = nullptr;
    explicit MainScene(QWidget *parent = nullptr);
    ~MainScene();
    void paintEvent(QPaintEvent *e);

private:
    Ui::MainScene *ui;
};

#endif // MAINSCENE_H
