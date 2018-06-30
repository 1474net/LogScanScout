#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString path = "C:/new/logs";
    QDate dt, yd;
    dt =  QDate::currentDate();
    yd = dt.addDays(-1);
    QString today = dt.toString("yyyy-MM-dd");
    QString yesterday = yd.toString("yyyy-MM-dd");
    qDebug() << today << yesterday ;

    QString terminal[3] = {"479800","479801","479806"};
    findFile(path, today, yesterday, terminal);



}
void MainWindow::findFile(QString path, QString today, QString yesterday, QString terminal[])
{
    qDebug() << terminal[2];
    QDir dir(path);
//    dir.setFilter( QDir::Dirs );
    dir.setSorting(QDir::Name);
    QFileInfoList folderitems ( dir.entryInfoList() );

    foreach ( QFileInfo i, folderitems ) {
            QString iname( i.fileName() );
            if ( iname == "." || iname == ".." || iname.isEmpty() )
                continue;

            if ( i.isDir() && iname == today || iname == yesterday )
                qDebug() << iname << "Дата найдена";
        }
}
MainWindow::~MainWindow()
{
    delete ui;
}
