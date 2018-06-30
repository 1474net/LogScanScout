#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QDebug>
#include <QDate>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void findFile(QString path, QString today=NULL, QString yesterday=NULL, QString terminal[]=NULL);

private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
