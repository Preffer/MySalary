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
    QStringList staff_headers;
    QStringList bonus_headers;
};

#endif // MYSALARY_H
