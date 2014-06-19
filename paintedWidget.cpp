#include "mysalary.h"
#include "ui_mysalary.h"

PaintedWidget::PaintedWidget()
{
    //resize(600, 400);
    //setWindowTitle("Paint Demo");
}

void PaintedWidget::paintEvent(QPaintEvent *event)
{
    int pointCount = 5;
    qDebug() << pointCount;
    QVector<cubicPoint> points(pointCount + 1);
    points[1] = cubicPoint(10, 390);
    points[2] = cubicPoint(200, 10);
    points[3] = cubicPoint(300, 340);
    points[4] = cubicPoint(400, 10);
    points[5] = cubicPoint(590, 390);

    points.append(points[pointCount-1] - points[pointCount-2] + points[pointCount]);

    QVector<QPointF> midPoints(pointCount+1);
    for(int i = 1; i <= pointCount; i++){
        midPoints[i] = (points[i].point + points[i+1].point) / 2;
    }
    //qDebug() << midPoints;

    QVector<QPointF> orentionPoints(pointCount);
    for(int i = 1; i < pointCount; i++){
        orentionPoints[i] = midPoints[i+1] - midPoints[i];
    }
    //qDebug() << orentionPoints;

    for(int i =1; i < pointCount; i++){
        points[i].afterPoint = points[i].point + orentionPoints[i]/2;
        points[i+1].beforePoint = points[i+1].point - orentionPoints[i]/2;
    }

    QPainterPath path;
    path.moveTo(points[1].point);
    for(int i = 1; i < pointCount; i++){
        path.cubicTo(points[i].afterPoint, points[i+1].beforePoint, points[i+1].point);
    }
    path.lineTo(points[1].point);

    QPainter painter(this);
    painter.setPen(QPen(QColor(151, 187, 205), 1, Qt::SolidLine,
                        Qt::FlatCap, Qt::RoundJoin));
    painter.setBrush(QColor(70, 216, 214));

    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPath(path);
}
