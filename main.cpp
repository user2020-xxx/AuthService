#include <QCoreApplication>
#include <web/httpserver.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    HttpServer server;
    server.start(8000);
    return a.exec();
}
