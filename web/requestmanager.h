#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>
#include <web/requestqueue.h>
#include <QStack>
#include <QThread>

class RequestManager : public QObject
{
    Q_OBJECT
    explicit RequestManager(QObject *parent = nullptr);
public:
    static RequestManager* Instance();

public slots:
    void proceedNextTask();

    void releaseThread();
    void handleResult(qint16 descriptor, const QString& result);
private:
    int _free_threads;
signals:
    void resultReady(qint16 descriptor, const QString& result);
    void operate();
};

#endif // REQUESTMANAGER_H
