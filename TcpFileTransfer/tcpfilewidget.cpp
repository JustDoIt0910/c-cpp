#include "tcpfilewidget.h"
#include "ui_tcpfilewidget.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QTimer>

TcpFileWidget::TcpFileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpFileWidget)
{
    ui->setupUi(this);
    setWindowTitle("服务器:8888");
    ui->btnSendFile->setEnabled(false);
    ui->btnSelectFile->setEnabled(false);
    timer = new QTimer(this);
    socket = nullptr;
    server = new QTcpServer(this);
    server->listen(QHostAddress::Any, 8888);

    connect(server, &QTcpServer::newConnection, this, [=](){
        socket = server->nextPendingConnection();
        QString clientIP = socket->peerAddress().toString();
        qint16 clientPort = qint16(socket->peerPort());
        QString message = QString("[%1:%2]成功连接").arg(clientIP).arg(clientPort);
        ui->textEdit->setText(message);
        ui->btnSelectFile->setEnabled(true);
    });

    connect(ui->btnSelectFile, &QPushButton::clicked, this, [=](){
        QString filePath = QFileDialog::getOpenFileName(this, "打开", "C:\\Users\\just do it\\Desktop");
        if(!filePath.isEmpty())
        {
            QFileInfo info(filePath);
            fileName = info.fileName();
            fileSize = info.size();
            sendSize = 0;

            file = new QFile;
            file->setFileName(filePath);
            bool ok = file->open(QIODevice::ReadOnly);
            if(!ok)
            {
                qDebug() << "打开文件失败";
                return;
            }
            ui->textEdit->append(filePath);
            ui->btnSendFile->setEnabled(true);
            ui->btnSelectFile->setEnabled(false);
        }

        connect(socket, &QTcpSocket::readyRead, this, [=](){
            QString message = QString(socket->readAll());
            if(message == "done")
            {
                ui->textEdit->append("文件发送完毕");
                file->close();
                socket->disconnectFromHost();
                socket->close();
            }
        });

    });

    connect(ui->btnSendFile, &QPushButton::clicked, this, [=](){
        if(socket == nullptr || fileName == "") return;
        QString head = QString("%1##%2").arg(fileName).arg(fileSize);
        qint64 len = socket->write(head.toUtf8().data());
        if(len > 0)
        {
            ui->textEdit->append("正在发送文件......");
            timer->start(20);
        }
        else
        {
            qDebug() << "头部信息发送失败";
            ui->btnSendFile->setEnabled(false);
            ui->btnSelectFile->setEnabled(true);
            file->close();
        }
    });

    connect(timer, &QTimer::timeout, this, [=](){
        timer->stop();
        sendData();
    });
}

void TcpFileWidget::sendData()
{
    qint64 len = 0;
    char buf[1024 * 4] = {0};
    do{
        len = file->read(buf, sizeof(buf));
        len = socket->write(buf, len);
        sendSize += len;
    }while(len > 0);
}


TcpFileWidget::~TcpFileWidget()
{
    delete ui;
}
