#ifndef UTIL_H
#define UTIL_H
#include <QDateTime>
#include <QString>

class Util
{
public:
    Util();
    static QString getTime() {
            return QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
    }
};

#endif // UTIL_H
