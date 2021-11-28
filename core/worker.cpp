#include "worker.h"
#include <qdebug.h>

Worker::Worker(qint16 descriptor, QString payload, QObject *parent) :
    QThread(parent),
    _descriptor(descriptor),
    _payload(payload)
{
    qDebug() << "worker payload: " << _payload;
}

void Worker::run()
{
    qDebug() << "run on descriptor " << _descriptor;
    QString result = _payload;

    qDebug() << "worker result: " << result;
    emit resultReady(_descriptor, result);
}
