#include "serverwidget.h"
#include "ui_serverwidget.h"


ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{

    ui->setupUi(this);
    setWindowTitle("TCP服务器:8888");
    tcpSocket = nullptr;
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any, 8888);
    connect(tcpServer, &QTcpServer::newConnection, this, [=](){
        tcpSocket = tcpServer->nextPendingConnection();
        QString ip = tcpSocket->peerAddress().toString();
        qint16 port = tcpSocket->peerPort();
        QString message = QString("[%1:%2]连接成功").arg(ip).arg(port);
        ui->readText->setText(message);

        connect(tcpSocket, &QTcpSocket::readyRead, this, [=](){
            QByteArray array = tcpSocket->readAll();
            ui->readText->append(array);
        });
    });

    connect(ui->btnSend, &QPushButton::clicked, this, [=](){
        if(tcpSocket == nullptr) return;
        QString message = ui->writeText->toPlainText();
        tcpSocket->write(message.toUtf8().data());
    });

    connect(ui->btnClose, &QPushButton::clicked, this, [=](){
        if(tcpSocket == nullptr) return;
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
        tcpSocket = nullptr;
    });
}

ServerWidget::~ServerWidget()
{
    delete ui;
}
