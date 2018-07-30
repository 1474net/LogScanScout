#ifndef SCAN_H
#define SCAN_H
#include <mainwindow.h>

class scan:public QObject
{
    Q_OBJECT
public:
    scan();
public slots:
    void doWork();
};

#endif // SCAN_H
