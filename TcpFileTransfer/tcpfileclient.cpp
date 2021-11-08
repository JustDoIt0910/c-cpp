#include "tcpfileclient.h"
#include "ui_tcpfileclient.h"
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>

TcpFileClient::TcpFileClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpFileClient), recvSize(0), isHead(true)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    file = new QFile(this);
    setWindowTitle("客户端");
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);

    connect(socket, &QTcpSocket::readyRead, this, [=](){
        QByteArray content = socket->readAll();
        if(isHead)
        {
            isHead = false;
            QString header = QString(content);
            fileName = header.section("##", 0, 0);
            fileSize = header.section("##", 1, 1).toInt();
            file->setFileName(fileName);
            bool ok = file->open(QIODevice::WriteOnly);
            if(!ok)
            {
                qDebug() << "open failed";
                return;
            }
            ui->labelName->setText(fileName);
            ui->labelSize->setText(QString("%1 KB").arg(fileSize / 1024));
        }
        else
        {
            qint64 len = file->write(content);
            recvSize += len;
            ui->progressBar->setValue(recvSize * 100 / fileSize);
            if(recvSize == fileSize)
            {
                socket->write("done");
                file->close();
                QMessageBox::information(this, "完成", "文件接收完成");
                socket->disconnectFromHost();
                socket->close();
            }
        }

    });

    connect(ui->btnConnect, &QPushButton::clicked, this, [=](){
        QString ip = ui->host->text();
        qint16 port = ui->port->text().toInt();
        socket->connectToHost(QHostAddress(ip), port);
    });
}

TcpFileClient::~TcpFileClient()
{
    delete ui;
}
