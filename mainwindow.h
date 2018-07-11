#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QDebug>
#include <QDate>
#include <QTextCodec>
#include <QTableWidget>
#include <QTimer>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QSettings>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
struct terminal{
    QString ID;
    QString NUMBER;
    int LINE;
};
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();




private:
    Ui::MainWindow *ui;
    QTimer* timer;
    QLineEdit *ledit1;
    QSettings *settings;
    QList<terminal> terminals;

    void findFile();
    void initTabmle();

    int count;
    QString today;
    QString yesterday;
    QString path = "";


private slots:
    void chektimer();
    void on_actionSettings_triggered();
    void on_actionEditPath();
    void on_actionInfo_triggered();
};

#endif // MAINWINDOW_H
