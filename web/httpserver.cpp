#include "httpserver.h"
#include <web/requestqueue.h>
#include <web/requestmanager.h>

HttpServer::HttpServer(QObject *parent) : QObject(parent)
{
    connect(RequestQueue::Instance(), &RequestQueue::newElement, RequestManager::Instance(), &RequestManager::proceedNextTask);
    connect(RequestManager::Instance(), &RequestManager::resultReady, this, &HttpServer::slotResponceReady);
}

HttpServer::~HttpServer()
{
    for( const auto& [_, socketsptr] : _socket_cache.toStdMap() )
    {
        QTcpSocket* socket = socketsptr.data();
        this->unbindSocket(socket);
    }
}

void HttpServer::start(quint16 port)
{
    if(!_server)
        _server = CreateServer();

    bool is_listen = _server->listen(QHostAddress::Any, port);
    if( is_listen )
    {
        connect(_server.data(), &QTcpServer::newConnection, this, &HttpServer::slotNewConnection);
        qDebug() << "server listen at the port " << port << Qt::endl;
    }
    else
    {
        qDebug() << "Unable to start server with port " << port << Qt::endl;
    }
}

QSharedPointer<QTcpServer> HttpServer::CreateServer()
{
    return QSharedPointer<QTcpServer>(new QTcpServer(this));
}

void HttpServer::unbindSocket(QTcpSocket *socket)
{
    disconnect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    disconnect(socket, &QTcpSocket::disconnected, this, &HttpServer::slotUnbindSocket);
    disconnect(socket, &QTcpSocket::readyRead, this, &HttpServer::slotBindSocket);
}

void HttpServer::slotNewConnection()
{
    QTcpSocket* socket =_server->nextPendingConnection();
    qDebug() << "new connection with descriptor " << socket->socketDescriptor();

    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    connect(socket, &QTcpSocket::disconnected, this, &HttpServer::slotUnbindSocket);
    connect(socket, &QTcpSocket::readyRead, this, &HttpServer::slotBindSocket);
}

void HttpServer::slotBindSocket()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    qint16 descriptor = socket->socketDescriptor();

    auto queue = RequestQueue::Instance();
    _socket_cache[descriptor] = QSharedPointer<QTcpSocket>(socket);
    queue->enqueue(descriptor, QString::fromLocal8Bit(socket->readAll()));
}

void HttpServer::slotUnbindSocket()
{
    auto socket = static_cast<QTcpSocket*>(sender());
    this->unbindSocket(socket);
}

void HttpServer::slotResponceReady(qint16 descriptor, const QString &result)
{
    qDebug() << "write result to socket " << descriptor << Qt::endl;
    auto socket = _socket_cache[descriptor];
    if(socket)
    {
        socket->write(result.toLocal8Bit());
        socket->flush();
    }
    else
    {
        qWarning() << "Unable to find socket with descriptor: " << descriptor << Qt::endl;
    }
}
