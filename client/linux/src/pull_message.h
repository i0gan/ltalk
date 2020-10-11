#ifndef PULLMESSAGE_H
#define PULLMESSAGE_H

#include <QObject>
#include <QThread>

class PullMessage : public QObject
{
    Q_OBJECT
public:
    explicit PullMessage(QObject *parent = nullptr);

signals:

};

#endif // PULLMESSAGE_H
