#include "px.h"
#include "mainwindow.h"
#include <QtGlobal>

PX::PX(QWidget *parent) : QWidget(parent)
{

}

void PX::paintEvent(QPaintEvent *e)

{

    Q_UNUSED(e);

    MainWindow *pwnd = (MainWindow *)this->parent();

    QPainter painter;

    QPen pen(Qt::red);

    painter.begin(this);    // start drawing
    int sz = qMin(e->rect().width(),e->rect().height());
    int w = sz / 21;
    int sx = 0;
    int sy = 0;

    painter.setPen(Qt::blue);
    for(int i=0;i<19;i++){
        painter.drawLine(sx+w, sy+i*w+w, sx+w*18+w,sy+ i*w+w);  // line
        painter.drawLine(sx+w+i*w, sy+w, sx+i*w+w, sy+w*18+w);  // line
        painter.drawText(sx,sy+i*w+w,QVariant(19-i).toString());
        painter.drawText(sx+w*i+w,sy+w*19+w,SGFStr.at(i));
    }
    for(int i=0;i<19;i++){
        for(int j=0;j<19;j++){
            if(pwnd->mPan[i][j]>0){
                if(pwnd->cPos == QPoint(j,i)){
                QRect r1 = QRect(sx+j*w+w/2,sy+i*w+w/2,w/2,w/2);
                painter.setBrush((pwnd->mPan[i][j]==1)? QBrush(Qt::blue) : QBrush(Qt::red));
                 painter.drawRect(r1);
                }
                QRect r = QRect(sx+j*w+w/2,sy+i*w+w/2,w,w);
                painter.setBrush((pwnd->mPan[i][j]==1)? QBrush(Qt::black) : QBrush(Qt::white));
                painter.drawEllipse(r);
            }
            if(pwnd->mMap[18-i][j]>0){
                painter.setBrush(Qt::transparent);
                QRect r = QRect(sx+j*w+w/2,sy+i*w+w/2,w,w);
                painter.drawText(r,QVariant(pwnd->mMap[18-i][j]).toString());
                painter.drawEllipse(r);
            }
        }
    }
      // rectangle
    painter.end();      // close Paint device

}

