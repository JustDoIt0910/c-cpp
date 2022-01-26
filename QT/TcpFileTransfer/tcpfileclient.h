#ifndef TCPFILECLIENT_H
#define TCPFILECLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>

namespace Ui {
class TcpFileClient;
}

class TcpFileClient : public QWidget
{
    Q_OBJECT

public:
    explicit TcpFileClient(QWidget *parent = nullptr);
    ~TcpFileClient();

private:
    Ui::TcpFileClient *ui;
    QTcpSocket *socket;

    QFile *file;
    QString fileName;
    qint64 fileSize;
    qint64 recvSize;

    bool isHead;
};

#endif // TCPFILECLIENT_H
