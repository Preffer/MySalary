#include "mysalary.h"
#include "ui_mysalary.h"
#include <QMessageBox>
#include <QSqlQueryModel>

MySalary::MySalary(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MySalary)
{
    ui->setupUi(this);
}

MySalary::~MySalary()
{
    delete ui;
}

void MySalary::on_loginButton_clicked()
{
    //init database
    if(!db.isOpen()){
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setPort(3306);
        db.setDatabaseName("qt");
        db.setUserName("qt");
        db.setPassword("erzX9fBsYFyJMfKB");
        db.setConnectOptions("CLIENT_SSL=1; CLIENT_IGNORE_SPACE=1");
        db.open();
    }

    //auth
    if(db.isOpen()){
        loginName = ui->username->text();

        QSqlQuery query;
        query.prepare("SELECT `staffID`, `name`, `password` FROM staff WHERE loginName = :loginName");
        query.bindValue(":loginName", loginName);
        if(query.exec()){
            query.first();
            if(query.isValid()){
                QString rawPass = ui->password->text();
                //qDebug() << rawPass;

                int index_password = query.record().indexOf("password");
                QString encodedPass = QString(QCryptographicHash::hash(rawPass.toUtf8(),QCryptographicHash::Md5).toHex());
                if(query.value(index_password).toString() == encodedPass){
                    int index_staffID = query.record().indexOf("staffID");
                    int index_name = query.record().indexOf("name");
                    name = query.value(index_name).toString();
                    staffID = query.value(index_staffID).toInt();

                    QMessageBox::information(this, "Success", "Login Success");
                    ui->stackedWidget->setCurrentIndex(1);
                    ui->tabWidget->setCurrentIndex(0);
                    this->on_tabWidget_currentChanged(0);

                } else{
                    QMessageBox::warning(this, "Failed", "Incorrect username/password");
                }
            } else{
                QMessageBox::warning(this, "Failed", "Incorrect username/password");
            }
        }else{
            qDebug() << query.lastError();
        }
    }
}

void MySalary::on_tabWidget_currentChanged(int index)
{
    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel();
    //qDebug() << index;
    switch(index){
    case 0:
        query.prepare("SELECT `date`, `salary` FROM salary WHERE staffID = :staffID");
        query.bindValue(":staffID", staffID);
        query.exec();

        model->setQuery(query);

        ui->salaryView->setModel(model);
        break;

    case 1:
        query.prepare("SELECT `timestamp`, `effect`, `note` FROM bonus WHERE staffID = :staffID");
        query.bindValue(":staffID", staffID);
        query.exec();

        model->setQuery(query);

        ui->bonusView->setModel(model);
        break;

    case 2:
        query.prepare("SELECT `info` FROM `staff` WHERE staffID = :staffID");
        query.bindValue(":staffID", staffID);

        if(query.exec()){
            query.first();
            QString rawPass = ui->password->text();
            int index_info = query.record().indexOf("info");
            QString info = query.value(index_info).toString();

            ui->new_info->setText(info);
            ui->new_loginName->setText(loginName);
            //qDebug() << info;
        }else{
            qDebug() << query.lastError();
        }
    }
}

void MySalary::on_infoCommitButton_clicked()
{
    QSqlQuery query;
    if(ui->new_password->isModified()){
        QString rawPass = ui->new_password->text();
        QString encodedPass = QString(QCryptographicHash::hash(rawPass.toUtf8(),QCryptographicHash::Md5).toHex());

        query.prepare("UPDATE `staff` SET `info` = :info, `loginName` = :loginName, `password` = :password WHERE staffID = :staffID");
        query.bindValue(":password", encodedPass);
    } else{
        query.prepare("UPDATE `staff` SET `info` = :info, `loginName` = :loginName WHERE staffID = :staffID");
    }
    query.bindValue(":info", ui->new_info->toPlainText());
    query.bindValue(":loginName", ui->new_loginName->text());
    query.bindValue(":staffID", staffID);

    if(query.exec()){
        QMessageBox::information(this, "Success", "Commit Success");
        this->on_tabWidget_currentChanged(2);
    }else{
        qDebug() << query.lastError();
    }
}
