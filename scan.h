#ifndef SCAN_H
#define SCAN_H
#include <mainwindow.h>

class scan:public QObject
{
    Q_OBJECT
public:
    MainWindow* w;
    scan(MainWindow* wind): w(wind){}
    QString pars(QFile * file, int line);
    int line;

public slots:
    void find();
signals:
    void send(QString,int);
};

#endif // SCAN_H
