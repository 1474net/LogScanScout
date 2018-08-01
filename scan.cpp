#include "scan.h"

void scan::pars()
{
    foreach (MainWindow::terminal ter, w->terminals) {
        QFile file(w->path+"/"+w->today+"/terminal_"+ter.ID+"_5ea09afb-0467-4e2f-b9a5-fa59d62d8720/"+"Debug.txt");
        if (file.exists())
        {
            emit send("NESOSY");
        }
        else
            emit send("SOSY");
    }

}
