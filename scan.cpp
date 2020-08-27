#include "scan.h"

void scan::find()
{

    QDir dir(w->path);
    QStringList dirictory = dir.entryList(QStringList());

    QString lastDir = dirictory[dirictory.count()-1];

    qDebug() << "Сканирование директории: "+lastDir;
    qDebug()<< "\n\n\n\nЗапуск сканирования\n################# ";

    line =0;
    for (int i=0;i<w->count; i++) {
       QDir dir(w->path+"/"+lastDir+"/_protocol/_terminals/");

       QStringList dirictoryTerminal = dir.entryList(QStringList());

       foreach(QString terminalFolder, dirictoryTerminal){
           if (terminalFolder.contains(w->terminals[i].ID)){
                QFile file(w->path+"/"+lastDir+"/_protocol/_terminals/"+terminalFolder+"/"+"Trace.txt");
                if (file.exists()){
                   emit send(pars(&file, w->terminals[i].LINE),i);
                   file.close();
                   w->terminals[i].LINE=line;
               }
           }
       }
    }
}

QString scan::pars(QFile *file, int line){

        QString logDate="";
        QString logTime="";
        QByteArray str = QByteArray::fromHex("cde5eff0e0e2e8ebfcedeee520f1eeeee1f9e5ede8e5204b4f4e5f544d5f484f53544b4e4620eef220d3cad2d121");
        int line_i = 0;
        qDebug()<<line;
        if (file->open(QIODevice::ReadOnly)){
            while(!file->atEnd()){
                    if(line_i<line){
                        str=file->readLine();
                        line_i++;
                        continue;
                    }
//                  Перекодировка из UTF8 в win125
                    str=file->readLine();
                    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
                    QString strf = codec->toUnicode(str);
//                  обрезаем лишнее из строки
                    QString newStr = strf.left(str.indexOf(';'));
//                  Ищем строку с датой и временим

                    if (newStr.contains("Время сообщения (UTC): ")){
                       logTime = newStr.remove("Время сообщения (UTC): ");

                    }
                    line_i++;
                }
                this->line=line_i;
        }
        return logTime;
}
