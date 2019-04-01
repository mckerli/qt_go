
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QLineEdit>
#include <QtGui>
#include "px.h"
#include "cdot.h"

namespace Ui {


class MainWindow;
}
extern QString SGFStr; // = "ABCDEFGHJKLMNOPQRST";

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     QProcess *myProcess;
     QString pText;
     void procData(QByteArray data);
     void updateBar(QPointF);

     void sendPlay(QPointF pos);
     void sendAnalyze();
     void sendUndo();
     void sendCommand(QString cmd);
     void procInfo(QString);
     QString mHeatmapStr;
     int mMap[19][19];
     int mPan[19][19];
     QPoint cPos;
     bool bSending = false;
     bool bSGF = false;
     void procSGF();
     QString mSGF;
     bool cTurn,bTurn = true;

     bool bProc = false;
     QList<CDot *> panList, mapList;
     QStringList lstPan;
     QTimer *timer, *timer2;
     bool getTurn();
     unsigned int num = 0;
     //virtual void paintEvent(QPaintEvent *);
     void resizeEvent(QResizeEvent *);
     void start_proc();
     bool eventFilter(QObject *obj, QEvent *event);
     PX *px;
    void makeMapBoard();
    void makePanBoard();
private slots:
    void updateProc();
    void readSTDBuffer();
    void readSTDError();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void processError(QProcess::ProcessError error);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
