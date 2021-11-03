#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QHostAddress>


ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    setWindowTitle("TCP客户端");
    tcpSocket = new QTcpSocket(this);
    connect(ui->btnConnect, &QPushButton::clicked, this, [=](){
        QString ip = ui->lineEditIp->text();
        qint16 port = ui->lineEditPort->text().toInt();
        tcpSocket->connectToHost(QHostAddress(ip), port);
    });

    connect(tcpSocket, &QTcpSocket::connected, this, [=](){
        ui->textRead->setText("成功连接服务器");
    });

    connect(ui->btnSend, &QPushButton::clicked, this, [=](){
        QString message = ui->textWrite->toPlainText();
        tcpSocket->write(message.toUtf8().data());
    });

    connect(tcpSocket, &QTcpSocket::readyRead, this, [=](){
        QByteArray array = tcpSocket->readAll();
        ui->textRead->append(array);
    });

    connect(ui->btnClose, &QPushButton::clicked, this, [=](){
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
    });
}

ClientWidget::~ClientWidget()
{
    delete ui;
}
