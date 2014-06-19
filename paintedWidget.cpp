#include "mysalary.h"
#include "ui_mysalary.h"

PaintedWidget::PaintedWidget()
{
    resize(600, 480);
    setWindowTitle("Chart");
    QFont sans("Microsoft YaHei");
    this->setFont(sans);
}

void PaintedWidget::paintEvent(QPaintEvent *event)
{
    int pointCount = data.size();
    //qDebug() << pointCount;
    QVector<cubicPoint> points(pointCount + 1);
    points[0] = cubicPoint(0, 480);
    //qDebug() << pointCount;
    QMap<QString, float>::const_iterator i = data.constBegin();
    this->max = this->min = i.value();
    while (i != data.constEnd())  {
        if(i.value() > this->max){
            this->max = i.value();
        }
        if(i.value() < this->min){
            this->min = i.value();
        }
        ++i;
    }
    i = data.constBegin();
    int current = 1;
    while (i != data.constEnd())  {
        points[current] = cubicPoint(600 / (pointCount-1) * (current-1), (450 - ((i.value() - this->min) / (this->max - this->min) * 420)));
        qDebug() << points[current].point;
        ++i;
        ++current;
    }
    points.append(points[pointCount-1] - points[pointCount-2] + points[pointCount]);
    qDebug() << points[current].point;
    qDebug() << "next";
    QVector<QPoint> midPoints(pointCount+1);
    for(int i = 1; i <= pointCount; i++){
        midPoints[i] = (points[i].point + points[i+1].point) / 2;
    }
    //qDebug() << midPoints;

    QVector<QPoint> orentionPoints(pointCount);
    for(int i = 1; i < pointCount; i++){
        orentionPoints[i] = midPoints[i+1] - midPoints[i];
    }
    //qDebug() << orentionPoints;

    for(int i =0; i < pointCount; i++){
        points[i].afterPoint = points[i].point + orentionPoints[i]/2;
        points[i+1].beforePoint = points[i+1].point - orentionPoints[i]/2;
    }

    QPainterPath path;
    path.moveTo(0, 480);
    for(int i = 0; i < pointCount; i++){
        path.cubicTo(points[i].afterPoint, points[i+1].beforePoint, points[i+1].point);
    }
    path.lineTo(points[pointCount].point.x(), 480);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(151, 187, 205), 3, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(QColor(70, 216, 214));
    painter.drawPath(path);

    i = data.constBegin();
    current = 1;
    while (i != data.constEnd())  {
        painter.setPen(QPen(QColor(255, 255, 255), 8, Qt::SolidLine,
                            Qt::RoundCap, Qt::RoundJoin));
        painter.drawPoint(points[current].point);

        painter.setPen(QPen(QColor(216, 45, 90), 5, Qt::SolidLine,
                            Qt::RoundCap, Qt::RoundJoin));
        painter.drawText(points[current].point, QString("%1").arg(i.value()));
        i++;
        current++;
    }
}
