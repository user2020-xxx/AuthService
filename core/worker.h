#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>

class Worker : public QThread
{
    Q_OBJECT
public:
    explicit Worker(qint16 descriptor, QString payload, QObject *parent = nullptr);

private:
    qint16 _descriptor;
    QString _payload;

    void run() override;
signals:
    void resultReady(qint16 descriptor, const QString& result);
};

#endif // WORKER_H
