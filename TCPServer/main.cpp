#include "serverwidget.h"
#include "clientwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerWidget server;
    ClientWidget client;
    server.show();
    client.show();

    return a.exec();
}
