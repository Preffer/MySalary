#ifndef MYSALARY_H
#define MYSALARY_H

#include <QMainWindow>
#include <QtSql>
#include <QPainter>

namespace Ui {
class MySalary;
}

class MySalary : public QMainWindow
{
    Q_OBJECT

public:
    explicit MySalary(QWidget *parent = 0);
    ~MySalary();

private slots:
    void on_loginButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_infoCommitButton_clicked();

    void on_grade_CommitButton_clicked();

    void on_grade_CancelButton_clicked();

    void on_grade_AddButton_clicked();

    void on_grade_DeleteButton_clicked();

    void on_grade_ASCButton_clicked();

    void on_grade_DESCButton_clicked();

    void on_staff_CommitButton_clicked();

    void on_staff_CancelButton_clicked();

    void on_staff_AddButton_clicked();

    void on_staff_DeleteButton_clicked();

    void on_staff_ASCButton_clicked();

    void on_staff_DESCButton_clicked();

    void on_filter_editingFinished();

    void on_bonus_CommitButton_clicked();

    void on_bonus_CancelButton_clicked();

    void on_bonus_AddButton_clicked();

    void on_bonus_DeleteButton_clicked();

    void on_bonus_ASCButton_clicked();

    void on_bonus_DESCButton_clicked();

    void on_bonus_filter_editingFinished();

    void on_admin_payButton_clicked();

    void on_admin_CommitButton_clicked();

    void on_admin_CancelButton_clicked();

    void on_admin_AddButton_clicked();

    void on_admin_DeleteButton_clicked();

    void on_admin_ASCButton_clicked();

    void on_admin_DESCButton_clicked();

    void on_admin_filter_editingFinished();

    void on_salaryChartButton_clicked();

    void on_bonusChartButton_clicked();

private:
    Ui::MySalary *ui;
    QSqlDatabase db;
    QString loginName;
    QString name;
    int staffID;
    QSqlQuery query;
    QSqlQueryModel *model;
    QSqlTableModel *gradeModel;
    QSqlRelationalTableModel *staffModel;
    QSqlRelationalTableModel *bonusModel;
    QSqlRelationalTableModel *adminModel;
    QStringList staff_headers;
    QStringList bonus_headers;
    QStringList admin_headers;
};

class PaintedWidget : public QWidget
{

public:
    PaintedWidget();
    QMap<QString, float> data;
    float max, min;

protected:
    void paintEvent(QPaintEvent *event);
};

class cubicPoint
{
public:
    QPoint point;
    QPoint beforePoint;
    QPoint afterPoint;

    cubicPoint(){};
    cubicPoint(int x, int y){
        point = QPoint(x, y);
    };
    cubicPoint(QPoint xy){
        point = QPoint(xy.x(), xy.y());
    };
    cubicPoint operator+ (cubicPoint & p2){
        return cubicPoint((this->point.x() + p2.point.x()), (this->point.y() + p2.point.y()));
    };
    cubicPoint operator- (cubicPoint & p2){
        return cubicPoint((this->point.x() - p2.point.x()), (this->point.y() - p2.point.y()));
    };
    cubicPoint operator* (int factor){
        return cubicPoint((this->point.x() * factor), (this->point.y() * factor));
    };
    cubicPoint operator/ (int factor){
        return cubicPoint((this->point.x() / factor), (this->point.y() / factor));
    };
};
#endif // MYSALARY_H
