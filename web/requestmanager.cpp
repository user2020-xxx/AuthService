#include "requestmanager.h"
#include <core/worker.h>

namespace {
const int MAX_THREAD_COUNT = 5;
}

RequestManager::RequestManager(QObject *parent) :
    QObject(parent),
    _free_threads(MAX_THREAD_COUNT)
{
}

RequestManager *RequestManager::Instance()
{
    static RequestManager* _manager;
    if(!_manager)
        _manager = new RequestManager();

    return _manager;
}

void RequestManager::proceedNextTask()
{
    RequestQueue* queue = RequestQueue::Instance();
    if(!_free_threads || queue->empty())
    {
        qDebug() << " has free threads: " << _free_threads << " and queue is empty: " << queue->empty() << Qt::endl;
        return;
    }
    auto [descriptor, payload] = queue->dequeue();
    qDebug() << "started thread with descriptor " << descriptor;

    Worker* worker = new Worker(descriptor, payload, this);

    connect(worker, &Worker::finished, worker, &Worker::deleteLater);
    connect(worker, &Worker::finished, this, &RequestManager::releaseThread);
    connect(worker, &Worker::resultReady, this, &RequestManager::handleResult);
    worker->start();

    _free_threads--;
}

void RequestManager::releaseThread()
{
    qDebug() << "new thread released";
    _free_threads++;
    proceedNextTask();
}

void RequestManager::handleResult(qint16 descriptor, const QString &result)
{
    emit resultReady(descriptor, result);
}

