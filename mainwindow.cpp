#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scan.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this -> setFixedSize(481,441);

    timer = new QTimer(this);
    timer->start(10000);
    connect(timer, SIGNAL(timeout()), this, SLOT(chektimer()));

    settings = new QSettings("ScanLogScout", "1474net");
    path = settings->value("data/path","").toString();

//    Дата
    QDate dt, yd;
    dt =  QDate::currentDate();
    yd = dt.addDays(-1);
    today = dt.toString("yyyy-MM-dd_");
    yesterday = yd.toString("yyyy-MM-dd");

    readFileterminals();
    initTabmle();


}

void MainWindow::chektimer(){
    qDebug()<< "\n\n\n\nЗапуск проверки\n################# ";
    findFile();

}

void MainWindow::readFileterminals(){
//    Чтение из фала список терминалов и создание структуры
    QFile file(QCoreApplication::applicationDirPath()+"/terminals.txt");
    if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
    {
        while(!file.atEnd()){

            QByteArray str = QByteArray::fromHex("cde5eff0e0e2e8ebfcedeee520f1eeeee1f9e5ede8e5204b4f4e5f544d5f484f53544b4e4620eef220d3cad2d121");
            str = file.readLine();
            QTextCodec *codec = QTextCodec::codecForName("UTF-8");
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
        //qDebug()<< "error file not exist" <<file;
    }
//    Подчет количества Элементов
    count = terminals.size();
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


    QImage imgRed;
    imgRed = QImage(":/ico/red.png").scaled(24,24,  Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

//    QImage imgGreen;
//    imgGreen = QImage(":/ico/green.png").scaled(24,24,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


    int i=0;
    while (i<count){
        ui->tableWidget->insertRow(i);

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem());
        QTableWidgetItem *wigItem = new QTableWidgetItem;
        wigItem->setData(Qt::DecorationRole, QPixmap::fromImage(imgRed));
        ui->tableWidget->setItem(i, 0, wigItem);

        ui->tableWidget->setItem(i,1, new QTableWidgetItem);
        ui->tableWidget->item(i,1)->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(i,1, new QTableWidgetItem());
        ui->tableWidget->item(i,1)->setText( terminals[i].ID);

        ui->tableWidget->setItem(i,2, new QTableWidgetItem());
        ui->tableWidget->item(i,2)->setText( terminals[i].NUMBER);
        ui->tableWidget->setItem(i,3, new QTableWidgetItem());
        i++;
    }
    ui->tableWidget->removeRow(i);

//        Сортировка
//    ui->tableWidget->resizeColumnsToContents();
//    ui->tableWidget->setSortingEnabled(true);


}


void MainWindow::findFile()
{
//    Фоновый парсинг файла
    scan *sc= new scan(this);
    QThread *thread= new QThread;
    sc->moveToThread(thread);

    connect(sc, SIGNAL(send(QString,int)), this, SLOT(update(QString,int)));
    connect(thread, SIGNAL(started()), sc, SLOT(find()));

    thread->start();


}

void MainWindow::update(QString str, int i){
    if(str!=""){
    QStringRef subString(&str, 0, 10);
    QString logDate = subString.toString();

    subString = QStringRef(&str, 11, 5);
    QString logTime = subString.toString();

    qDebug() << "Дата" << logDate << logTime;

    // Обновление данных таблицы
        ui->tableWidget->item(i,3)->setText(str);

    //    Цвет табличек
        if (logDate==QDate().currentDate().toString("dd.MM.yyyy")&&logDate!=""){
            if (logTime[0]==QTime::currentTime().toString()[0]){
                if (logTime[1]==":")
                    ui->tableWidget->item(i,3)->setBackgroundColor(Qt::green);
                else
                    if (logTime[1]==QTime::currentTime().toString()[1])
                        ui->tableWidget->item(i,3)->setBackgroundColor(Qt::green);
                    else
                        ui->tableWidget->item(i,3)->setBackgroundColor(Qt::yellow);
            }
            else {
                ui->tableWidget->item(i,3)->setBackgroundColor(Qt::yellow);
            }
        }
        else if (logDate!=""){
            ui->tableWidget->item(i,3)->setBackgroundColor(Qt::red);
        }
    }
//    Проверка статуса загрузки терминалов и изменение цвета иконки
    qDebug() << terminals[i].DATE;

    if(terminals[i].DATE!=str && str!=" ")
    {
            QImage imgGreen;
            imgGreen = QImage(":/ico/green.png").scaled(24,24,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            QTableWidgetItem *wigItem = new QTableWidgetItem;
            wigItem->setData(Qt::DecorationRole, QPixmap::fromImage(imgGreen));
            ui->tableWidget->setItem(i, 0, wigItem);
            terminals[i].DATE=str;
    }
    else {
        QImage imgRed;
        imgRed = QImage(":/ico/red.png").scaled(24,24,  Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QTableWidgetItem *wigItem = new QTableWidgetItem;
        wigItem->setData(Qt::DecorationRole, QPixmap::fromImage(imgRed));
        ui->tableWidget->setItem(i, 0, wigItem);
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
