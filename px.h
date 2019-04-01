#ifndef PX_H
#define PX_H

#include <QWidget>
#include <QtGui>
//#include "mainwindow.h"
extern QString SGFStr;
class PX : public QWidget
{
    Q_OBJECT
public:
    explicit PX(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *) override;


signals:

public slots:
};

#endif // PX_H
