#ifndef TCPFILEWIDGET_H
#define TCPFILEWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <Qtimer>


namespace Ui {
class TcpFileWidget;
}

class TcpFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TcpFileWidget(QWidget *parent = nullptr);
    void sendData();
    ~TcpFileWidget();

private:
    Ui::TcpFileWidget *ui;
    QTcpServer *server;
    QTcpSocket *socket;

    QFile *file;
    QString fileName;
    qint64 fileSize;
    qint64 sendSize;
    QTimer *timer;
};

#endif // TCPFILEWIDGET_H
