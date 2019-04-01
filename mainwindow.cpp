#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDebug>
#include <QtGlobal>

QString SGFStr = "ABCDEFGHJKLMNOPQRST";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    bTurn = true;
    bSGF = false;
    mSGF = "";
    memset(mPan,0,sizeof(mPan));
    memset(mMap,0,sizeof(mMap));

    px = new PX(this);
    start_proc();
    ui->setupUi(this);

    qApp->installEventFilter(this);
 }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeMapBoard(){

    memset(mMap,0,sizeof(mMap));
    for(int i=0;i<mapList.length();i++){
        CDot *dot = mapList.at(i);
        mMap[dot->pos.y()][dot->pos.x()] = dot->jum;
    }
    qInfo() << "ok " << mapList.length() <<  endl;
    mapList.clear();
}
void MainWindow::makePanBoard(){

    memset(mPan,0,sizeof(mPan));
    for(int i=0;i<panList.length();i++){
        CDot *dot = panList.at(i);
        mPan[dot->pos.y()][dot->pos.x()] = dot->jum;
    }
    qInfo() << "ok " << panList.length() <<  endl;

}
bool MainWindow::getTurn(){
    if(lstPan.length()==0) return false;
    QString st = lstPan.last();
    return (st.at(0)=='B');
}
void MainWindow::procSGF(){
    QStringList lst = mSGF.split(";");
    lstPan.clear();
    QString chs = "abcdefghijklmnopqrstu";
    //memset(mPan,0,sizeof(mPan));
    for(int i=0;i<lst.length();i++){
        QString str = lst.at(i);
        if(str.length()>=5 &&str.at(1)=="[" && str.at(4)=="]"){
            lstPan << str.left(5);
        }
    }
    bSending = false;
    if(lstPan.length()==0){
        bTurn = true;
    }
    else{
        bTurn = (lstPan.last().at(0)=='W');
    }

    this->setCursor(Qt::ArrowCursor);
    px->update();
    //sendAnalyze();
    qInfo() << "list of sgf turn=" << bTurn;
}
void MainWindow::procInfo(QString str){
    QStringList kk = str.split("info move ");
    mapList.clear();
    for(int i=0;i<kk.length();i++){
        QStringList pt = kk.at(i).trimmed().split(" ");
        if(pt.length()>= 11) mapList << new CDot(pt);
    }
    makeMapBoard();
    px->update();
}
void MainWindow::procData(QByteArray data){

    pText +=  QString::fromLocal8Bit(data);
    int n;
    while((n = pText.indexOf("\n"))>=0){
        QString str = pText.left(n);
        int len = str.length();
        pText = pText.remove(0,n+1);
        qInfo() << str << len;
        /*
        if(str.length()>7 && str.mid(4,3)==" ->"){
            CDot *dot = new CDot(str);
            list << dot;
            qInfo() << dot->pos << " " << dot->jum << " " << dot->ptStr;

        }
        */
        if(str.startsWith("info move")){
            procInfo(str);
        }
        /*
        else if(str.startsWith("= (;")){
            bSGF = true;
            mSGF = str;
        }
        else if(bSGF){
            if(str.endsWith(")")){
                mSGF += str;
                bSGF = false;
                procSGF();
                qInfo() << mSGF;
            }else{
                mSGF += str;
            }
        }
        */
        else if(bSending){
            if(str.startsWith("White time: ")){
                bSending = false;

                if(!cTurn) bTurn = true;
                this->setCursor(Qt::ArrowCursor);
                px->update();
                sendAnalyze();
                qInfo() << "list of sgf turn=" << bTurn;
            }
            else{
                for(int i=1;i<=19;i++){
                    QString cc;
                    cc.sprintf("%2d",i);
                    if(str.startsWith(cc) && str.endsWith(cc) && str.length()==43){
                        for(int j=0;j<=18;j++){
                            QString x = str.at(2+j*2+1);
                            QString c = str.at(2+j*2);
                            mPan[19-i][j] = (x=='X') ? 1 : (x=='O' ? 2 : 0);
                            if(c == '('){
                                bTurn = (x=='O');
                                cTurn = true;
                                cPos = QPoint(j,19-i);
                            }
                        }
                    }
                }
            }

        }

    };


}
bool MainWindow::eventFilter(QObject *o, QEvent *e)
{
    if(e->type()==QEvent::MouseMove){
                QMouseEvent *me = static_cast<QMouseEvent*>(e);
                updateBar(me->windowPos());
                return true;
    }
    if(e->type()==QEvent::MouseButtonRelease){
        QMouseEvent *me = static_cast<QMouseEvent*>(e);
        if(me->button() == Qt::LeftButton){
            sendPlay(me->windowPos());
            return true;
        }else if(me->button() == Qt::RightButton){
            sendUndo();
            return true;
        }
    }
    return false;
}
void MainWindow::sendCommand(QString cmd){
     myProcess->write(cmd.toLocal8Bit());
}

void MainWindow::sendAnalyze(){
    QString st = (getTurn()) ? "lz-analyze w 10\n" : "lz-analyze b 10\n";
    myProcess->write(st.toLocal8Bit());
}

void MainWindow::sendUndo(){
    bSending = true;
    cTurn = false;
    this->setCursor(Qt::WaitCursor);
    QString cmd = "undo\nshowboard\n";
    myProcess->write(cmd.toUtf8());
}
void MainWindow::sendPlay(QPointF pos){
    int sz = px->width() / 21;
    if(sz==0){
      //  statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(pos.x()-px->x()).arg(pos.y()-px->y()));
        return;
    }
    int sx = pos.x() - px->x() - sz/2;
    int sy = pos.y() - px->y() - sz/2;
    if(sx<0 || sy < 0){
        //statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(pos.x()-px->x()).arg(pos.y()-px->y()));

        return;
    }
    int cx = sx / sz;
    int cy = sy / sz;

    if(cx>18 || cy>18){
        //statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(pos.x()-px->x()).arg(pos.y()-px->y()));

        return;
    }
    if( mPan[cy][cx]!=0) {
        qInfo() << "alreay occupied v=" << mPan[cy][cx] << " cx=" << cx << " cy=" << cy;
        return;
    }
    mPan[cy][cx] = (bTurn) ? 1 : 2;

    QString cmd = QString("%1%2").arg(SGFStr.at(cx)).arg(19-cy);
    bSending = true;
    cTurn = false;
    this->setCursor(Qt::WaitCursor);
    cmd = ((bTurn) ? "\nplay b " : "\nplay w ")+cmd+"\nshowboard\n";
    myProcess->write(cmd.toUtf8());

    px->update();

}
void MainWindow::updateBar(QPointF pos){
    int sz = px->width() / 21;
    if(sz==0){
        statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(pos.x()-px->x()).arg(pos.y()-px->y()));
        return;
    }
    int sx = pos.x() - px->x() - sz/2;
    int sy = pos.y() - px->y() - sz/2;
    if(sx<0 || sy < 0){
        statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(pos.x()-px->x()).arg(pos.y()-px->y()));

        return ;
    }
    int cx = sx / sz;
    int cy = sy / sz;

    if(cx>18 || cy>18){
        statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(pos.x()-px->x()).arg(pos.y()-px->y()));

        return;
    }

    statusBar()->showMessage(QString("Mouse move (%1,%2), (%3,%4), (%5,%6)").arg(pos.x()-px->x()).arg(pos.y()-px->y()).arg(cx).arg(cy).arg(SGFStr.at(cx)).arg(19-cy));
    //return QString("%1%2").arg(SGFStr.at(cx)).arg(19-cy);
}

void MainWindow::readSTDBuffer(){

    //qInfo() << "buffer";
    QByteArray newData = myProcess->readAllStandardOutput();
    procData(newData);

    //qList.append(text);
    //qInfo() <<  qList.length();
    //qInfo() << qList;

}
void MainWindow::readSTDError(){

    //qInfo() << "error";
    QByteArray newData = myProcess->readAllStandardError();
    procData(newData);
}
void MainWindow::start_proc()
{
    //

    qInfo() << "Button clicked";

   // QObject *parent;

    QString program = "/home/mck/leela-zero/build/leelaz";
    QStringList arguments;
    arguments << "-w" << "network.gz" << "--gtp" << "--lagbuffer" << "0" << "-t" << "8";

    myProcess = new QProcess(this);
    myProcess->setWorkingDirectory("/home/mck/leela-zero/build/");
    connect(myProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readSTDBuffer()));
    connect( myProcess, SIGNAL(readyReadStandardError()), this, SLOT(readSTDError()));
    connect( myProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)) );
    connect( myProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

    myProcess->start(program, arguments);
    sendAnalyze();
}
void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
    {
    qInfo() << exitCode << exitStatus;
    }
void MainWindow::processError(QProcess::ProcessError error)
{
    qInfo() << error;
}
void MainWindow::updateProc(){

}


void MainWindow::resizeEvent(QResizeEvent *e){

    int sz = qMin(e->size().width(),e->size().height());
    int w = sz / 21;
    int sx = (e->size().width() - w*19)/2;
    int sy = (e->size().height() - w*19)/2;
    px->move(sx,sy);
    px->resize(w*20,w*20);
};


