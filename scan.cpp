#include "scan.h"

void scan::find()
{
    foreach (MainWindow::terminal ter, w->terminals) {
        QFile file(w->path+"/"+w->today+"/terminal_"+ter.ID+"_5ea09afb-0467-4e2f-b9a5-fa59d62d8720/"+"Debug.txt");
        if (file.exists()){
            emit send(pars(&file, &ter));
            file.close();
        }
        else{
            QFile file(w->path+"/"+w->yesterday+"/terminal_"+ter.ID+"_5ea09afb-0467-4e2f-b9a5-fa59d62d8720/"+"Debug.txt");
            if (file.exists()){
                emit send(pars(&file, &ter));
                file.close();
            }
            else {
                emit send("SOSY");
            }

        }
    }
}

QString scan::pars(QFile *file, MainWindow::terminal * terminal){

        QString logDate="";
        QString logTime="";
        QByteArray str = QByteArray::fromHex("cde5eff0e0e2e8ebfcedeee520f1eeeee1f9e5ede8e5204b4f4e5f544d5f484f53544b4e4620eef220d3cad2d121");
        int line_i;
        qDebug()<<terminal->LINE;
        if (file->open(QIODevice::ReadOnly))
            while(!file->atEnd()){
                    if(line_i<terminal->LINE){
                        str=file->readLine();
                        line_i++;
                        continue;
                    }
    //                                    Перекодировка из UTF8 в win125
                    str=file->readLine();
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
    //                   ui->tableWidget->item(j,2)->setText(logDate+" "+logTime);
                    }
                    line_i++;
                }
        terminal->LINE=line_i-1;

        return logDate+" "+logTime;
}
