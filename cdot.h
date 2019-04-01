#ifndef CDOT_H
#define CDOT_H

#include <QObject>
#include <QtGui>
extern QString SGFStr; // = "ABCDEFGHJKLMNOPQRST";
class CDot : public QObject
{
    Q_OBJECT
public:
    explicit CDot(QObject *parent = nullptr);
    QPoint pos;
    QStringList slst;
    int jum;

    CDot(QStringList s){
        slst = s;
        qInfo() << slst;
        jum = slst.at(2).toInt();
        conver();
    }
    void conver(){
       QString ptStr = slst.at(0);
       pos.setX(SGFStr.indexOf(ptStr.at(0)));
       pos.setY(ptStr.mid(1).toInt()-1);
    }
signals:

public slots:
};

#endif // CDOT_H


