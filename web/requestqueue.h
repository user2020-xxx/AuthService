#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include <QObject>
#include <QTcpSocket>
#include <QQueue>
#include <QMap>

class RequestQueue : public QObject
{
    Q_OBJECT
    explicit RequestQueue(QObject *parent = nullptr) : QObject(parent) {} // Пусто - выросла капуста!

public:
    static RequestQueue* Instance();

    void enqueue(qint16 index, QString payload);
    std::tuple<qint16, QString> dequeue();

    bool empty();

private:
    QQueue<qint16> _queue;
    QMap<qint16, QString> _payload_cache;

signals:
    void newElement();
};

#endif // REQUESTQUEUE_H
