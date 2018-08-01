#ifndef SCAN_H
#define SCAN_H
#include <mainwindow.h>

class scan:public QObject
{
    Q_OBJECT
public:
    MainWindow* w;
    scan(MainWindow* wind): w(wind){}



public slots:
    void pars();
signals:
    void send(QString);
};

#endif // SCAN_H
