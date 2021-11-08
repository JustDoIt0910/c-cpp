#include "tcpfilewidget.h"
#include "tcpfileclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpFileWidget server;
    TcpFileClient client;
    server.show();
    client.show();
    return a.exec();
}
