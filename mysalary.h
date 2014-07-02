/**
* @copyright 2012-2016, Zhejiang University
* @version 1.0.0
* @author <黄羽众 学号3120102663>
*/
/**
* @file mysalary.cpp
* @brief 企业工资管理系统
* 版本历史
* @date 2014-06-21*/
#ifndef MYSALARY_H
#define MYSALARY_H

#include <QMainWindow>
#include <QtSql>
#include <QPainter>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QFileDialog>
#include <QTextStream>

namespace Ui {
class MySalary;
}

class MySalary : public QMainWindow
{
    Q_OBJECT

public:
    explicit MySalary(QWidget *parent = 0);
    void exportToCSV(QString sql);
    void importFromCSV(QString sql);
    ~MySalary();

private slots:

/**
* @brief  连接数据库，并运用数据库检查登陆用户信息 
* @return 无 
* @exception 若连接出错或登陆信息有误均能报错 
* @note 暂无
*/
    void on_loginButton_clicked();
    
/**
* @brief  根据登陆的不同权限显示工资情况 
* @param [in] 输入参数1 : 登陆用户的权限水平 
* @return 无 
* @note 暂无
*
*/
    void on_tabWidget_currentChanged(int index);
    
/**
* @brief  修改个人备注 
* @return 无 
* @note 暂无
*/
    void on_infoCommitButton_clicked();
/**
* @brief  管理员修改 gradeID：确认按钮 
* @return 无 
* @exception 增加时若未填写gradeID或者ID重复会报错 
* @note 暂无
*/
    void on_grade_CommitButton_clicked();
/**
* @brief  管理员修改 gradeID：取消按钮 
* @return 无 
* @note 在没有按commit之前有效 
*/
    void on_grade_CancelButton_clicked();
/**
* @brief  管理员修改 gradeID：增加新的一行 
* @return 无 
* @note baseSalary默认为0 
*/
    void on_grade_AddButton_clicked();
/**
* @brief  管理员修改 gradeID：删除一行 
* @return 无 
* @note 会有提示操作是否继续 
*/
    void on_grade_DeleteButton_clicked();
/**
* @brief  grade升序排列 
* @return 无 
* @note 暂无
*/
    void on_grade_ASCButton_clicked();
/**
* @brief  grade降序排列 
* @return 无 
* @note 暂无
*/
    void on_grade_DESCButton_clicked();
/**
* @brief  管理员查询管理企业员工情报：确认按钮 
* @return 无 
* @exception 增加时若未完整或者staffID重复会报错 
* @note 暂无
*/
    void on_staff_CommitButton_clicked();
/**
* @brief  管理员查询管理企业员工情报：取消按钮
* @return 无 
* @note 在commit生效前有效  
*/
    void on_staff_CancelButton_clicked();
/**
* @brief  管理员查询管理企业员工情报：增加新员工 
* @return 无 
* @note 暂无
*/
    void on_staff_AddButton_clicked();
/**
* @brief  管理员查询管理企业员工情报：删除一个员工 
* @return 无 
* @note 会有提示操作是否继续 
*/
    void on_staff_DeleteButton_clicked();
/**
* @brief  管理员查询管理企业员工情报：升序查看 
* @return 无 
* @note 暂无
*/
    void on_staff_ASCButton_clicked();
/**
* @brief  管理员查询管理企业员工情报：降序查看 
* @return 无 
* @note 暂无
*/
    void on_staff_DESCButton_clicked();
/**
* @brief  管理员查询管理企业员工情报：修改完后及时显示 
* @return 无 
* @note 暂无
*/
    void on_filter_editingFinished();
/**
* @brief  财务人员奖惩功能：确认按钮 
* @return 无 
* @exception 增加时若未完整或者bonusID重复会报错 
* @note 暂无
*/
    void on_bonus_CommitButton_clicked();
/**
* @brief  财务人员奖惩功能：取消按钮 
* @return 无 
* @note 在commit生效前有效
*/
    void on_bonus_CancelButton_clicked();
/**
* @brief  财务人员奖惩功能：添加一行奖惩记录 
* @return 无 
* @note 奖惩金额默认为0
*/
    void on_bonus_AddButton_clicked();
/**
* @brief  财务人员奖惩功能：删除一行奖惩记录 
* @return 无 
* @note 会有提示操作是否继续 
*/
    void on_bonus_DeleteButton_clicked();
/**
* @brief  财务人员奖惩功能：奖惩记录升序排列显示 
* @return 无 
* @note 暂无
*/
    void on_bonus_ASCButton_clicked();
/**
* @brief  财务人员奖惩功能：奖惩记录升序排列显示 
* @return 无 
* @note 暂无
*/
    void on_bonus_DESCButton_clicked();
/**
* @brief  财务人员奖惩功能：修改完后及时显示 
* @return 无 
* @note 暂无
*/
    void on_bonus_filter_editingFinished();
/**
* @brief  企业管理：支付工资 
* @return 无 
* @note 提示将支付的各项信息 
*/
    void on_admin_payButton_clicked();
/**
* @brief  企业管理-管理人员管理  ：确认按钮
* @return 无 
* @exception 增加时若未完整或者adminID重复会报错 
* @note 暂无
*/
    void on_admin_CommitButton_clicked();
/**
* @brief  企业管理-管理人员管理 ：取消按钮 
* @return 无 
* @note 在commit生效前有效
*/
    void on_admin_CancelButton_clicked();
/**
* @brief  企业管理-管理人员管理 ：增加新的管理员 
* @return 无 
* @note暂无 
*/
    void on_admin_AddButton_clicked();
/**
* @brief  企业管理-管理人员管理 ：删除一个管理员
* @return 无 
* @note 会有提示操作是否继续  
*/
    void on_admin_DeleteButton_clicked();
/**
* @brief  企业管理-管理人员管理 ：升序显示 
* @return 无 
* @note 暂无
*/
    void on_admin_ASCButton_clicked();
/**
* @brief  企业管理-管理人员管理 ：降序显示 
* @return 无 
* @note 暂无
*/
    void on_admin_DESCButton_clicked();
/**
* @brief  企业管理-管理人员管理 ：修改后及时显示 
* @return 无 
* @note 暂无
*/
    void on_admin_filter_editingFinished();
/**
* @brief  个人工资变化图 
* @return 无 
* @note 工资为Y轴，时间为X轴 
*/
    void on_salaryChartButton_clicked();
/**
* @brief  个人奖惩变化图 
* @return 无 
* @note 金额为Y轴，时间为X轴 
*/
    void on_bonusChartButton_clicked();

/**
* @brief  企业工资变化图 
* @return 无 
* @note 金额为Y轴，时间为X轴 
*/
    void on_salaryChartButton_2_clicked();

    void on_exportGradeButton_clicked();

    void on_exportStaffButton_clicked();

    void on_exportBonusButton_clicked();

    void on_exportSalaryButton_clicked();

    void on_exportAdminButton_clicked();

    void on_importGradeButton_clicked();

    void on_importStaffButton_clicked();

    void on_importBonusButton_clicked();

    void on_importSalaryButton_clicked();

    void on_importAdminButton_clicked();

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
