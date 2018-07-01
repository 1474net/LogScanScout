#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Путь к файлу
    QString path = "C:/new/logs";
    //Дата
    QDate dt, yd;
    dt =  QDate::currentDate();
    yd = dt.addDays(-1);
    QString today = dt.toString("yyyy-MM-dd");
    QString yesterday = yd.toString("yyyy-MM-dd");
    //Debug() << today << yesterday ;

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //Массив с терминалами
    QString terminal[1] = {"479800"};
    ui->tableWidget->setItem(0,0, new QTableWidgetItem());
    ui->tableWidget->item(0,0)->setText( terminal[0]);

    findFile(path, today, yesterday, terminal);
    qDebug() <<"2";

}


void MainWindow::findFile(QString path, QString today, QString yesterday, QString terminal[])
{
    //Сканирование папок на название текущей даты
    QDir logDir(path);
//    dir.setFilter( QDir::Dirs );
    logDir.setSorting(QDir::Name);
    QFileInfoList folderitems ( logDir.entryInfoList() );

    foreach ( QFileInfo i, folderitems ) {
            QString dateFold( i.fileName() );
            // Игнорируем папки текущей и предыдушей папки
            if ( dateFold == "." || dateFold == ".." || dateFold.isEmpty() )
                continue;
            // Сравниваем имена папок и текушею дату
            if ( i.isDir() && dateFold == today || dateFold == yesterday ){
                //TODO:Сделать поиск папок с названием терминалов
                qDebug() << dateFold << "Дата найдена";

                QDir termPath(path+"/"+dateFold);
                termPath.setSorting(QDir::Name);
                QFileInfoList folderitems_t ( termPath.entryInfoList() );
                int j;
                foreach (QFileInfo t, folderitems_t) {    
                    QString name_term( t.fileName() );
                    j=0;
                    if ( name_term == "." || name_term == ".." || name_term.isEmpty() )
                        continue;

                    while(j<sizeof( terminal)/ sizeof( terminal[0])){
                        if (name_term.contains(terminal[j])){
                            qDebug()<< name_term << "Терминал найден" << terminal[j];
                            QFile file(path+"/"+dateFold+"/"+name_term+"/"+"Debug.txt");
                            if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
                            {
                                while(!file.atEnd())
                                    {

                                        QByteArray str = QByteArray::fromHex("cde5eff0e0e2e8ebfcedeee520f1eeeee1f9e5ede8e5204b4f4e5f544d5f484f53544b4e4620eef220d3cad2d121");
                                        str=file.readLine();
                                        QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
                                        QString strf = codec->toUnicode(str);
                                        QString newStr = strf.left(str.indexOf(';'));
                                        if (newStr.contains("Время сообщения:")){
                                           QStringRef subString(&newStr, 17, 10);
                                           QString logDate = subString.toString();
                                           subString = QStringRef(&newStr, 28, 5);
                                           QString logTime = subString.toString();
                                           qDebug() << logDate << "Лох" << logTime << "Loh" ;
                                           ui->tableWidget->item(0,0)->setText(logDate);
                                        }

                                    }

                                file.close();
                            }
                            break;
                        }
                        j++;
                    }

                }
            }
        }

}
MainWindow::~MainWindow()
{
    delete ui;
}
