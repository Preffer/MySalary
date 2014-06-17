#ifndef MYSALARY_H
#define MYSALARY_H

#include <QMainWindow>
#include <QtSql>


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

private:
    Ui::MySalary *ui;
    QSqlDatabase db;
    QString loginName;
    QString name;
    int staffID;
    QSqlQuery query;
    QSqlQueryModel *model;
};

#endif // MYSALARY_H
