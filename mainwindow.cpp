#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this -> setFixedSize(481,441);

    timer = new QTimer(this);
    timer->start(10000);
    connect(timer, SIGNAL(timeout()), this, SLOT(chektimer()));

    settings = new QSettings("ScanLogScout", "1474net");
    path = settings->value("data/path","").toString();
//    Дата
    QDate dt, yd;
    dt =  QDate::currentDate();
    yd = dt.addDays(-1);
    today = dt.toString("yyyy-MM-dd");
    yesterday = yd.toString("yyyy-MM-dd");
//    Debug() << today << yesterday ;

    QFile file(QCoreApplication::applicationDirPath()+"/terminals.txt");
    if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
    {
        while(!file.atEnd()){

            QByteArray str = QByteArray::fromHex("cde5eff0e0e2e8ebfcedeee520f1eeeee1f9e5ede8e5204b4f4e5f544d5f484f53544b4e4620eef220d3cad2d121");
            str = file.readLine();
            QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
            QString strf = codec->toUnicode(str);

            QStringList list2 = strf.split('\t', QString::SkipEmptyParts);
            qDebug()<<list2[0] <<list2[1].simplified();

            terminal Terminal;
            Terminal.ID=list2[0];
            Terminal.NUMBER=list2[1];
            Terminal.LINE=0;
            terminals.append(Terminal);
        }
    }
    else {
        qDebug()<< "error file not exist" <<file;
    }

    count = terminals.size();
    initTabmle();


}

void MainWindow::chektimer(){
    qDebug()<< "checktime"<< count;
    findFile();

}

void MainWindow::initTabmle(){
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
        ui->tableWidget->item(i,0)->setText( terminals[i].ID);

        ui->tableWidget->setItem(i,1, new QTableWidgetItem());
        ui->tableWidget->item(i,1)->setText( terminals[i].NUMBER);
        ui->tableWidget->setItem(i,2, new QTableWidgetItem());
        i++;
    }
    ui->tableWidget->removeRow(i);

}

void MainWindow::findFile()
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
//                        qDebug()<< name_term << "Проверяем теримал";
//                         Проверяем на совпадения Входит ли имя терминала в название папки
                        if (name_term.contains(terminals[j].ID)){
                            qDebug()<< name_term << "Терминал найден" << terminals[j].ID << j <<"count";
//                             Если совпал то проверяем наличие файла логов
                            QFile file(path+"/"+dateFold+"/"+name_term+"/"+"Debug.txt");
                            if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
                            {
                                QString logDate="";
                                QString logTime="";
                                QByteArray str = QByteArray::fromHex("cde5eff0e0e2e8ebfcedeee520f1eeeee1f9e5ede8e5204b4f4e5f544d5f484f53544b4e4620eef220d3cad2d121");
                                int line_i=0;
                                //Чтения файла 
                                while(!file.atEnd()){
                                        if(line_i<terminals[j].LINE){
                                            str=file.readLine();
                                            line_i++;
                                            continue;
                                        }
//                                    Перекодировка из UTF8 в win125
                                        str=file.readLine();
                                        QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
                                        QString strf = codec->toUnicode(str);
//                                        обрезаем лишнее из строки
                                        QString newStr = strf.left(str.indexOf(';'));
//                                       Ищем строку с датой и временим
                                        if (newStr.contains("Время сообщения:")){
                                           QStringRef subString(&newStr, 17, 10);
                                           logDate = subString.toString();
                                           subString = QStringRef(&newStr, 28, 5);
                                           logTime = subString.toString();
//                                          Записываем в данные в таблицу
                                           ui->tableWidget->item(j,2)->setText(logDate+" "+logTime);
                                        }
                                        line_i++;

                                    }
                                file.close();
                                terminals[j].LINE=line_i-1;
                                if (logDate==QDate().currentDate().toString("dd.MM.yyyy")&&logDate!=""){
                                    if (logTime[0]==QTime::currentTime().toString()[0])
                                        if (logTime[1]==":")
                                            ui->tableWidget->item(j,2)->setBackgroundColor(Qt::green);
                                        else
                                            if (logTime[1]==QTime::currentTime().toString()[1])
                                                ui->tableWidget->item(j,2)->setBackgroundColor(Qt::green);
                                            else
                                                ui->tableWidget->item(j,2)->setBackgroundColor(Qt::yellow);
                                }
                                else if (logDate!=""){
                                    ui->tableWidget->item(j,2)->setBackgroundColor(Qt::red);
                                    qDebug() << QDate().currentDate().toString("dd.MM.yyyy") << logDate;
                                    qDebug() <<terminals[j].LINE;
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

void MainWindow::on_actionSettings_triggered()
{
    QDialog dlg(this);
    dlg.setWindowTitle(tr("Настройки"));
    dlg.setFixedSize(220,100);

    ledit1 = new QLineEdit(&dlg);
    if ( path=="")
        ledit1->setText("Нажмите для выбора");
    else
        ledit1->setText(path);
    QPushButton * btn = new QPushButton;
    btn->setFixedSize(100,25);
    btn->setText("Открыть");
    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    connect(btn, SIGNAL( pressed()), this, SLOT(on_actionEditPath()));

    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("Путь:"), ledit1);
    layout->addRow(btn);
    layout->setAlignment(btn, Qt::AlignHCenter);
    layout->addWidget(btn_box);

    dlg.setLayout(layout);

    // В случае, если пользователь нажал "Ok".
    if(dlg.exec() == QDialog::Accepted) {
        const QString &str1 = ledit1->text();
    }
}
void MainWindow::on_actionEditPath(){
        QString str = QFileDialog::getExistingDirectory();
        path = str;
        ledit1->setText(str);
        settings->setValue("data/path",path);
        settings->sync();

}

void MainWindow::on_actionInfo_triggered()
{
    QMessageBox::aboutQt(this);
}
