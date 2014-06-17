#include "mysalary.h"
#include "ui_mysalary.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>

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
                QString encodedPass;

                if(rawPass == ""){
                    //hack here to allow new user login
                    encodedPass = rawPass;
                } else{
                    encodedPass = QString(QCryptographicHash::hash(rawPass.toUtf8(),QCryptographicHash::Md5).toHex());
                }
                int index_password = query.record().indexOf("password");
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
        query.prepare("SELECT `staffID`, `name`, `info`, `gradeName`, `loginName`  FROM `staff` JOIN `grade` ON `staffID` = :staffID AND `grade`.`gradeID` = `staff`.`gradeID`");
        query.bindValue(":staffID", staffID);

        if(query.exec()){
            query.first();
            int index_staffID = query.record().indexOf("staffID");
            int index_name = query.record().indexOf("name");
            int index_gradeName = query.record().indexOf("gradeName");
            int index_info = query.record().indexOf("info");
            int index_loginName = query.record().indexOf("loginName");

            ui->staffID->setText(query.value(index_staffID).toString());
            ui->name->setText(query.value(index_name).toString());
            ui->gradeName->setText(query.value(index_gradeName).toString());
            ui->new_info->setText(query.value(index_info).toString());
            ui->new_loginName->setText(query.value(index_loginName).toString());
            //qDebug() << info;
        }else{
            qDebug() << query.lastError();
        }
        break;

    case 3:
        gradeModel = new QSqlTableModel();
        gradeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        gradeModel->setTable("grade");
        gradeModel->select();

        ui->gradeView->setModel(gradeModel);
        break;

    case 4:
        staffModel = new QSqlRelationalTableModel();
        staffModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        staffModel->setTable("staff");
        staffModel->setRelation(3, QSqlRelation("grade", "gradeID", "gradeName"));
        staffModel->select();

        ui->staffView->setModel(staffModel);
        //ui->staffView->hideColumn(5);
        ui->staffView->setItemDelegate(new QSqlRelationalDelegate());

        QStringList headers;
        for(int i = 0; i < staffModel->columnCount(); i++){
             headers << staffModel->headerData(i, Qt::Horizontal).toString();
        }

        ui->filter_comboBox->addItems(headers);
        qDebug() << headers;
        break;
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

void MySalary::on_grade_CommitButton_clicked()
{
    gradeModel->database().transaction();
    if (gradeModel->submitAll()) {
        gradeModel->database().commit();
    } else {
        gradeModel->database().rollback();
        QMessageBox::warning( this, "Commit Error", gradeModel->lastError().text());
    }
}

void MySalary::on_grade_CancelButton_clicked()
{
    gradeModel->revertAll();
}

void MySalary::on_grade_AddButton_clicked()
{
    int rowNum = gradeModel->rowCount();
    gradeModel->insertRow(rowNum); //index of the new row is equal to the rowCount
}

void MySalary::on_grade_DeleteButton_clicked()
{
    int curRow = ui->gradeView->currentIndex().row();
    gradeModel->removeRow(curRow);
    int confirm = QMessageBox::warning(this, "Delete Current Row", "Are you sure!!\nDelete current row?", QMessageBox::Yes,QMessageBox::No);
    if(confirm == QMessageBox::No)
    {
        gradeModel->revertAll();
    }
    else gradeModel->submitAll();
}

void MySalary::on_grade_ASCButton_clicked()
{
    gradeModel->setSort(0,Qt::AscendingOrder);
    gradeModel->select();
}

void MySalary::on_grade_DESCButton_clicked()
{
    gradeModel->setSort(0,Qt::DescendingOrder);
    gradeModel->select();
}

void MySalary::on_staff_CommitButton_clicked()
{
    staffModel->database().transaction();
    if (staffModel->submitAll()) {
        staffModel->database().commit();
    } else {
        staffModel->database().rollback();
        QMessageBox::warning( this, "Commit Error", staffModel->lastError().text());
    }
}

void MySalary::on_staff_CancelButton_clicked()
{
    staffModel->revertAll();
}

void MySalary::on_staff_AddButton_clicked()
{
    int rowNum = staffModel->rowCount();
    staffModel->insertRow(rowNum); //index of the new row is equal to the rowCount
}

void MySalary::on_staff_DeleteButton_clicked()
{
    int curRow = ui->staffView->currentIndex().row();
    staffModel->removeRow(curRow);
    int confirm = QMessageBox::warning(this, "Delete Current Row", "Are you sure!!\nDelete current row?", QMessageBox::Yes,QMessageBox::No);
    if(confirm == QMessageBox::No)
    {
        staffModel->revertAll();
    }
    else staffModel->submitAll();
}

void MySalary::on_staff_ASCButton_clicked()
{
    int flag = ui->filter_comboBox->currentIndex();
    staffModel->setSort(flag,Qt::AscendingOrder);
    staffModel->select();
}

void MySalary::on_staff_DESCButton_clicked()
{
    int flag = ui->filter_comboBox->currentIndex();
    staffModel->setSort(flag,Qt::DescendingOrder);
    staffModel->select();
}

void MySalary::on_staffView_clicked(const QModelIndex &index)
{
    /*QVariant value;
    value = "ok";
    if(index.column() == 0){
        staffModel->setData(index, value);
    }*/

    qDebug() << index;
}
