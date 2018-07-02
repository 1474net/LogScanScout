#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    timer->start(10000);

//    Путь к файлу
    path = "C:/new/logs";
//    Дата
    QDate dt, yd;
    dt =  QDate::currentDate();
    yd = dt.addDays(-1);
    today = dt.toString("yyyy-MM-dd");
    yesterday = yd.toString("yyyy-MM-dd");
//    Debug() << today << yesterday ;

//    Массив с терминалами

    count = sizeof(terminal)/sizeof(terminal[0]);
    connect(timer, SIGNAL(timeout()), this, SLOT(chektimer()));


    initTabmle(count, terminal);


}

void MainWindow::chektimer(){
    qDebug()<< "Sosy"<< count;
    findFile(path, today, yesterday, terminal);

}

void MainWindow::initTabmle(int count, QString terminal[]){
    ui->tableWidget->setShowGrid(true); // Включаем сетку
// Разрешаем выделение только одного элемента
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
// Разрешаем выделение построчно
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
// Растягиваем последнюю колонку на всё доступное пространство
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
// Запрешаем редактировать
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
// Запрешаем растягивать
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    int i=0;
    while (i<count){
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0, new QTableWidgetItem());
        ui->tableWidget->item(i,0)->setText( terminal[i]);

        ui->tableWidget->setItem(i,1, new QTableWidgetItem());
        ui->tableWidget->setItem(i,1, new QTableWidgetItem());
        ui->tableWidget->setItem(i,2, new QTableWidgetItem());
        ui->tableWidget->setItem(i,2, new QTableWidgetItem());
        i++;
    }
    ui->tableWidget->removeRow(i);

}

void MainWindow::findFile(QString path, QString today, QString yesterday, QString terminal[])
{
//    Сканирование папок на название текущей даты
    QDir logDir(path);
//    dir.setFilter( QDir::Dirs );
    logDir.setSorting(QDir::Name);
    QFileInfoList folderitems ( logDir.entryInfoList() );

    foreach ( QFileInfo i, folderitems ) {
            QString dateFold( i.fileName() );
//             Игнорируем папки текущей и предыдушей папки
            if ( dateFold == "." || dateFold == ".." || dateFold.isEmpty() )
                continue;
//             Сравниваем имена папок и текушею дату
            if ( i.isDir() && dateFold == today || dateFold == yesterday ){
                qDebug() << dateFold << "Дата найдена";
                
//                 проверяем наличие папок с именами терминалов
                QDir termPath(path+"/"+dateFold);
                termPath.setSorting(QDir::Name);
                QFileInfoList folderitems_t ( termPath.entryInfoList() );
                int j;

                foreach (QFileInfo t, folderitems_t) {
                    j=0;
                    QString name_term( t.fileName() );
//                     Счетчик для перебора массива с терминалами
                    if ( name_term == "." || name_term == ".." || name_term.isEmpty() )
                        continue;
//                    Цикл проверки имен терминалов
                    while(j<=count){
                        qDebug()<< name_term << "Проверяем теримал";
//                         Проверяем на совпадения Входит ли имя терминала в название папки
                        if (name_term.contains(terminal[j])){
                            qDebug()<< name_term << "Терминал найден" << terminal[j] << j <<"count";
//                             Если совпал то проверяем наличие файла логов
                            QFile file(path+"/"+dateFold+"/"+name_term+"/"+"Debug.txt");
                            if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
                            {
                                QString logDate="";
                                QString logTime="";
                                //Чтения файла 
                                while(!file.atEnd())
                                    {
//                                    Перекодировка из UTF8 в win1251
                                        QByteArray str = QByteArray::fromHex("cde5eff0e0e2e8ebfcedeee520f1eeeee1f9e5ede8e5204b4f4e5f544d5f484f53544b4e4620eef220d3cad2d121");
                                        str=file.readLine();
                                        QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
                                        QString strf = codec->toUnicode(str);
//                                        обрезаем лишнее из строки
                                        QString newStr = strf.left(str.indexOf(';'));
//                                        Ищем строку с датой и временим
                                        if (newStr.contains("Время сообщения:")){
                                           QStringRef subString(&newStr, 17, 10);
                                           logDate = subString.toString();
                                           subString = QStringRef(&newStr, 28, 5);
                                           logTime = subString.toString();
//                                          qDebug() << logDate << "Лох" << logTime << "Loh" << j ;
//                                           Записываем в данные в таблицу
                                           ui->tableWidget->item(j,2)->setText(logDate+" "+logTime);

                                        }

                                    }
                                file.close();
                                if (logDate=="02.07.2018")
                                    ui->tableWidget->item(j,2)->setBackgroundColor(Qt::green);
                                else {
                                    ui->tableWidget->item(j,2)->setBackgroundColor(Qt::red);
                                }
                                break;
                            }

                        }
                        else{
                          j++;
                        }

                    }

                }
            }
        }

}
MainWindow::~MainWindow()
{
    delete ui;
}
