#include "scan.h"

scan::scan()
{

}
void scan::doWork()
{
for(int i = 0; i <= 10000; i++)
{
    for(int j = 0; j <= 10000; j++)
    {
        qDebug() << j;

    }
    qDebug() << i;

}
}
