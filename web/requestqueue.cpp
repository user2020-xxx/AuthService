#include "requestqueue.h"
#include <qdebug.h>

RequestQueue *RequestQueue::Instance()
{
    static RequestQueue* _queue;
    if(!_queue)
        _queue = new RequestQueue();

    return _queue;
}

void RequestQueue::enqueue(qint16 index, QString payload)
{
    _queue.enqueue(index);
    _payload_cache[index] = payload;

    qDebug() << "[" << index << "] insert payload";
    emit newElement();
}

std::tuple<qint16, QString> RequestQueue::dequeue()
{
   auto index = _queue.dequeue();

   std::tuple<qint16, QString> payload = {index, _payload_cache[index]};
   _payload_cache.remove(index);

   qDebug() << "[" << index << "] dequed payload";
   return payload;
}

bool RequestQueue::empty()
{
    return _queue.empty();
}
