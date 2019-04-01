#include "cmthread.h"
#include <QDebug>
CMThread::CMThread(QObject *parent)
{
    mwin = (QMainWindow *)parent;
}
void CMThread::run()
{
    MainWindow *pwin = (MainWindow *)mwin;
    while(true)
    {
        pwin->list.clear();
        QByteArray cmd = "lz-analyze\n\n";
        pwin->myProcess->write(cmd);
        sleep(1000);

    }
}
