#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QDebug>
#include <QDate>
#include <QTextCodec>
#include <QTableWidget>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();




private:
    Ui::MainWindow *ui;
    QTimer* timer;

    void findFile(QString path, QString today, QString yesterday, QString terminal[]);
    void initTabmle(int count, QString terminal[]=NULL);

    QString terminal[12] = {"479808","479810","479801","479806","479807",
                                              "479800","479811","479816",
                                     "479817","479818","479822","479823"};
    int count;
    QString today;
    QString yesterday;
    QString path;


private slots:
    void chektimer();



};

#endif // MAINWINDOW_H
