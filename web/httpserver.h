#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QTcpServer>
#include <QQueue>
#include <QMap>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = nullptr);
    ~HttpServer();

    void start(quint16 port);

private:
    QSharedPointer<QTcpServer> CreateServer();
    QSharedPointer<QTcpServer> _server;

protected:
    QMap<qint16, QSharedPointer<QTcpSocket>> _socket_cache;

    void unbindSocket(QTcpSocket* socket);

public slots:
    void slotNewConnection();

    void slotBindSocket();
    void slotUnbindSocket();

    void slotResponceReady(qint16 descriptor, const QString& result);
signals:
    void signalNewQueue(qint16 descriptor);
};

#endif // HTTPSERVER_H
