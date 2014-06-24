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
#include "mysalary.h"
#include "ui_mysalary.h"


MySalary::MySalary(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MySalary)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    QStringList drivers = QSqlDatabase::drivers();
    if(!drivers.contains("QMYSQL", Qt::CaseInsensitive)){
        QMessageBox::critical(this, "MySQL Driver Not Found", "MySQL Driver Not Found");
    }
}

MySalary::~MySalary()
{
    delete ui;
}
/**
* @brief  连接数据库，并运用数据库检查登陆用户信息 
*/
void MySalary::on_loginButton_clicked()
{
    //init database
    if(!db.isOpen()){
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("10.202.82.90");
        db.setPort(3306);
        db.setDatabaseName("qt");
        db.setUserName("qt");
        db.setPassword("erzX9fBsYFyJMfKB");
        db.setConnectOptions("CLIENT_SSL=1; CLIENT_IGNORE_SPACE=1");
        db.open();
    } else{
        qDebug() << db.lastError().text();
    }

    //auth
    if(db.isOpen()){
        loginName = ui->username->text();

        QSqlQuery query;
        query.prepare("SELECT `staff`.`staffID`, `name`, `password`, `privilege` FROM staff LEFT JOIN `admin` ON `staff`.`staffID` = `admin`.`staffID` WHERE `loginName` = :loginName");
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
                    int index_privilege = query.record().indexOf("privilege");
                    int privilege = query.value(index_privilege).toInt();
                    if(privilege < 60){
                        ui->tabWidget->removeTab(3);
                        ui->tabWidget->removeTab(4);
                        ui->tabWidget->removeTab(5);
                        ui->tabWidget->removeTab(6);
                        ui->tabWidget->removeTab(7);
                    } else{
                        if(privilege < 100){
                            ui->tabWidget->removeTab(7);
                        }
                    }
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
/**
* @brief  根据登陆的不同权限显示工资情况 
*/
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

        for(int i = 0; i < staffModel->columnCount(); i++){
            staff_headers << staffModel->headerData(i, Qt::Horizontal).toString();
        }

        ui->filter_comboBox->addItems(staff_headers);
        //qDebug() << staff_headers;
        break;

    case 5:
        bonusModel = new QSqlRelationalTableModel();
        bonusModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        bonusModel->setTable("bonus");
        bonusModel->setRelation(1, QSqlRelation("staff", "staffID", "name"));
        bonusModel->select();

        ui->admin_bonus_View->setModel(bonusModel);
        ui->admin_bonus_View->setItemDelegate(new QSqlRelationalDelegate());

        for(int i = 0; i < bonusModel->columnCount(); i++){
            bonus_headers << bonusModel->headerData(i, Qt::Horizontal).toString();
        }

        ui->bonus_filter_comboBox->addItems(bonus_headers);
        //qDebug() << bonus_headers;
        break;

    case 6:
        query.prepare("SELECT `date`, SUM(salary) as sum FROM `salary` GROUP BY `date`");
        query.exec();

        model->setQuery(query);
        ui->admin_salaryView->setModel(model);

        ui->admin_salary_dateEdit->setDate(QDate::currentDate());
        break;

    case 7:
        adminModel = new QSqlRelationalTableModel();
        adminModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        adminModel->setTable("admin");
        adminModel->setRelation(2, QSqlRelation("staff", "staffID", "name"));
        adminModel->select();

        ui->admin_View->setModel(adminModel);
        ui->admin_View->setItemDelegate(new QSqlRelationalDelegate());

        for(int i = 0; i < adminModel->columnCount(); i++){
            admin_headers << adminModel->headerData(i, Qt::Horizontal).toString();
        }

        ui->admin_filter_comboBox->addItems(admin_headers);
        //qDebug() << bonus_headers;
        break;
    }
}
/**
* @brief  修改个人备注 
*/
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
/**
* @brief  管理员修改 gradeID：确认按钮 
*/
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
/**
* @brief  管理员修改 gradeID：取消按钮 
*/
void MySalary::on_grade_CancelButton_clicked()
{
    gradeModel->revertAll();
}
/**
* @brief  管理员修改 gradeID：增加新的一行 
*/
void MySalary::on_grade_AddButton_clicked()
{
    int rowNum = gradeModel->rowCount();
    gradeModel->insertRow(rowNum); //index of the new row is equal to the rowCount
}
/**
* @brief  管理员修改 gradeID：删除一行 
*/
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
/**
* @brief  grade升序排列 
*/
void MySalary::on_grade_ASCButton_clicked()
{
    gradeModel->setSort(0,Qt::AscendingOrder);
    gradeModel->select();
}
/**
* @brief  grade降序排列 
*/
void MySalary::on_grade_DESCButton_clicked()
{
    gradeModel->setSort(0,Qt::DescendingOrder);
    gradeModel->select();
}
/**
* @brief  管理员查询管理企业员工情报：确认按钮 
*/
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
/**
* @brief  管理员查询管理企业员工情报：取消按钮 
*/
void MySalary::on_staff_CancelButton_clicked()
{
    staffModel->revertAll();
}
/**
* @brief  管理员查询管理企业员工情报：增加新员工 
*/
void MySalary::on_staff_AddButton_clicked()
{
    int rowNum = staffModel->rowCount();
    staffModel->insertRow(rowNum); //index of the new row is equal to the rowCount
}
/**
* @brief  管理员查询管理企业员工情报：删除一个员工 
*/
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
/**
* @brief  管理员查询管理企业员工情报：升序查看 
*/
void MySalary::on_staff_ASCButton_clicked()
{
    int flag = ui->filter_comboBox->currentIndex();
    staffModel->setSort(flag,Qt::AscendingOrder);
    staffModel->select();
}
/**
* @brief  管理员查询管理企业员工情报：降序查看 
*/
void MySalary::on_staff_DESCButton_clicked()
{
    int flag = ui->filter_comboBox->currentIndex();
    staffModel->setSort(flag,Qt::DescendingOrder);
    staffModel->select();
}

/**
* @brief  管理员查询管理企业员工情报：修改完后及时显示 
*/
void MySalary::on_filter_editingFinished()
{
    QString keyword = ui->filter->text();
    QString filed = ui->filter_comboBox->currentText();
    //qDebug() << keyword;
    staffModel->setFilter(filed + " LIKE '%" + keyword + "%'"); //just where clause in SQL
    staffModel->select();
}
/**
* @brief  财务人员奖惩功能：确认按钮 
*/
void MySalary::on_bonus_CommitButton_clicked()
{
    bonusModel->database().transaction();
    if (bonusModel->submitAll()) {
        bonusModel->database().commit();
    } else {
        bonusModel->database().rollback();
        QMessageBox::warning( this, "Commit Error", bonusModel->lastError().text());
    }
}
/**
* @brief  财务人员奖惩功能：取消按钮 
*/
void MySalary::on_bonus_CancelButton_clicked()
{
    bonusModel->revertAll();
}
/**
* @brief  财务人员奖惩功能：添加一行奖惩记录 
*/
void MySalary::on_bonus_AddButton_clicked()
{
    int rowNum = bonusModel->rowCount();
    bonusModel->insertRow(rowNum); //index of the new row is equal to the rowCount
}
/**
* @brief  财务人员奖惩功能：删除一行奖惩记录 
*/
void MySalary::on_bonus_DeleteButton_clicked()
{
    int curRow = ui->staffView->currentIndex().row();
    bonusModel->removeRow(curRow);
    int confirm = QMessageBox::warning(this, "Delete Current Row", "Are you sure!!\nDelete current row?", QMessageBox::Yes,QMessageBox::No);
    if(confirm == QMessageBox::No)
    {
        bonusModel->revertAll();
    }
    else bonusModel->submitAll();
}
/**
* @brief  财务人员奖惩功能：奖惩记录升序排列显示 
*/
void MySalary::on_bonus_ASCButton_clicked()
{
    int flag = ui->bonus_filter_comboBox->currentIndex();
    bonusModel->setSort(flag,Qt::AscendingOrder);
    bonusModel->select();
}
/**
* @brief  财务人员奖惩功能：奖惩记录降序排列显示 
*/
void MySalary::on_bonus_DESCButton_clicked()
{
    int flag = ui->bonus_filter_comboBox->currentIndex();
    bonusModel->setSort(flag,Qt::DescendingOrder);
    bonusModel->select();
}
/**
* @brief  财务人员奖惩功能：修改完后及时显示 
*/
void MySalary::on_bonus_filter_editingFinished()
{
    QString keyword = ui->bonus_filter->text();
    QString filed = ui->bonus_filter_comboBox->currentText();
    //qDebug() << keyword;
    staffModel->setFilter(filed + " LIKE '%" + keyword + "%'"); //just where clause in SQL
    staffModel->select();
}
/**
* @brief  企业管理：支付工资 
*/
void MySalary::on_admin_payButton_clicked()
{
    QDate date = ui->admin_salary_dateEdit->date ();
    //qDebug() << date.toString("yyyy-MM");
    QVariant bonusCount, bonusSUM, commonCount, commonSUM, total;
    QSqlQuery query;
    QString sql;
    sql = "SELECT COUNT(*) AS count, SUM(effect) AS sum FROM `bonus` WHERE timestamp LIKE'" + date.toString("yyyy-MM") + "%'";
    if(query.exec(sql)){
        query.first();
        bonusCount = query.value(0).toDouble();
        bonusSUM = query.value(1).toDouble();
        //qDebug() << bonusSUM;
    } else{
        qDebug() << query.lastError();
    }
    sql = "SELECT COUNT(*) AS count, SUM(baseSalary) AS sum FROM `staff` JOIN `grade` ON `grade`.`gradeID` = `staff`.`gradeID`";
    if(query.exec(sql)){
        query.first();
        commonCount = query.value(0).toDouble();
        commonSUM = query.value(1).toDouble();
        //qDebug() << commonSUM;
    } else{
        qDebug() << query.lastError();
    }
    total = bonusSUM.toDouble() + commonSUM.toDouble();
    QString tip = "Salary statistic in " + date.toString("yyyy-MM") + "\n";
    tip += bonusCount.toString() + " employees gain bonus with " + bonusSUM.toString() + " in total\n";
    tip += commonCount.toString() + " employees gain salary with " + commonSUM.toString() + " in total\n";
    tip += "To sum up, " + total.toString() + " to pay";
    int confirm = QMessageBox::question(this, "Confirm Pay Salary", tip, QMessageBox::Yes,QMessageBox::No);
    if(confirm == QMessageBox::Yes)
    {
        QMap<int, float> map;
        sql = "SELECT `staffID`, `baseSalary` FROM `staff` JOIN `grade` ON `staff`.`gradeID` = `grade`.`gradeID`";
        query.exec(sql);
        while(query.next()){
            map[query.value(0).toInt()] = query.value(1).toFloat();
        }
        //qDebug() << map;
        sql = "SELECT `staffID`, `effect` FROM `bonus` WHERE timestamp LIKE'" + date.toString("yyyy-MM") + "%'";
        query.exec(sql);
        while(query.next()){
            map[query.value(0).toInt()] += query.value(1).toFloat();
        }
        //qDebug() << map;

        query.prepare("INSERT INTO `salary`(`staffID`, `date`, `salary`) VALUES (:staffID, :date, :salary)");
        query.bindValue(":date", date.toString("yyyy-MM-dd"));

        QMap<int, float>::const_iterator i = map.constBegin();

        int tranStatus = 0;
        db.transaction();
        while (i != map.constEnd()) {
            query.bindValue(":staffID", i.key());
            query.bindValue(":salary", i.value());
            if(!query.exec()){
                tranStatus = 1;
            }
            ++i;
        }
        if(tranStatus == 0){
            db.commit();
            QMessageBox::information(this, "Success", "Confirm Pay Salary Success!");
        }else{
            db.rollback();
            QMessageBox::warning(this, "Failed", query.lastError().text());
        }
    }
}
/**
* @brief  企业管理-管理人员管理  ：确认按钮 
*/
void MySalary::on_admin_CommitButton_clicked()
{
    adminModel->database().transaction();
    if (adminModel->submitAll()) {
        adminModel->database().commit();
    } else {
        adminModel->database().rollback();
        QMessageBox::warning( this, "Commit Error", adminModel->lastError().text());
    }
}
/**
* @brief  企业管理-管理人员管理 ：取消按钮 
*/
void MySalary::on_admin_CancelButton_clicked()
{
    adminModel->revertAll();
}
/**
* @brief  企业管理-管理人员管理 ：增加新的管理员 
*/
void MySalary::on_admin_AddButton_clicked()
{
    int rowNum = adminModel->rowCount();
    adminModel->insertRow(rowNum); //index of the new row is equal to the rowCount
}
/**
* @brief  企业管理-管理人员管理 ：删除一个管理员 
*/
void MySalary::on_admin_DeleteButton_clicked()
{
    int curRow = ui->admin_View->currentIndex().row();
    adminModel->removeRow(curRow);
    int confirm = QMessageBox::warning(this, "Delete Current Row", "Are you sure!!\nDelete current row?", QMessageBox::Yes,QMessageBox::No);
    if(confirm == QMessageBox::No)
    {
        adminModel->revertAll();
    }
    else adminModel->submitAll();
}
/**
* @brief  企业管理-管理人员管理 ：升序显示 
*/
void MySalary::on_admin_ASCButton_clicked()
{
    int flag = ui->admin_filter_comboBox->currentIndex();
    adminModel->setSort(flag,Qt::AscendingOrder);
    adminModel->select();
}
/**
* @brief  企业管理-管理人员管理 ：降序显示 
*/
void MySalary::on_admin_DESCButton_clicked()
{
    int flag = ui->admin_filter_comboBox->currentIndex();
    adminModel->setSort(flag,Qt::DescendingOrder);
    adminModel->select();
}
/**
* @brief  企业管理-管理人员管理 ：修改后及时显示 
*/
void MySalary::on_admin_filter_editingFinished()
{
    QString keyword = ui->admin_filter->text();
    QString filed = ui->admin_filter_comboBox->currentText();
    //qDebug() << keyword;
    adminModel->setFilter(filed + " LIKE '%" + keyword + "%'"); //just where clause in SQL
    adminModel->select();
}
/**
* @brief  个人工资变化图 
*/
void MySalary::on_salaryChartButton_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT `date`, `salary` FROM salary WHERE staffID = :staffID");
    query.bindValue(":staffID", staffID);
    query.exec();

    QMap<QString, float> map;
    while(query.next()){
        map[query.value(0).toString()] = query.value(1).toFloat();
    }
    //qDebug() << map;
    PaintedWidget *paint = new PaintedWidget();
    paint->data = map;
    paint->show();
}

/**
* @brief  个人奖惩变化图 
*/
void MySalary::on_bonusChartButton_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT `timestamp`, `effect` FROM bonus WHERE staffID = :staffID");
    query.bindValue(":staffID", staffID);
    query.exec();

    QMap<QString, float> map;
    while(query.next()){
        map[query.value(0).toString()] = query.value(1).toFloat();
    }
    //qDebug() << map;
    PaintedWidget *paint = new PaintedWidget();
    paint->data = map;
    paint->show();
}

/**
* @brief  企业工资变化图 
*/
void MySalary::on_salaryChartButton_2_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT `date`, SUM(salary) as sum FROM `salary` GROUP BY `date`");
    query.exec();

    QMap<QString, float> map;
    while(query.next()){
        map[query.value(0).toString()] = query.value(1).toFloat();
    }
    //qDebug() << map;
    PaintedWidget *paint = new PaintedWidget();
    paint->data = map;
    paint->show();
}


void MySalary::saveToCSV(QString sql){
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "*.csv");

    if(fileName.lastIndexOf(".csv") == -1){
        fileName.append(".csv");
    }
    QSqlQuery query;
    query.prepare(sql);
    query.exec();
    QSqlRecord record = query.record();

    int fieldCount = record.count();
    QStringList fields, results;
    for(int i = 0; i < fieldCount; i++){
        fields << record.fieldName(i);
    }
    results << fields.join(",");
    int j = 0;
    while(query.next()){
        QStringList *rows = new QStringList;
        for(j = 0; j < fieldCount; j++){
            *rows << query.value(j).toString();
        }
        results << (*rows).join(",");
        delete rows;
    }
    qDebug() << results.join("\n");

    QFile file(fileName);
    if(!file.open(QIODevice::Text | QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Failed", "Opening file failed!");
    } else
    {
        QTextStream stream(&file);
        stream << results.join("\n");
        file.close();
        QMessageBox::information(this, "Success", "Save to CSV file success!");
        qDebug() << "wrote file to " + fileName;
    }
}


void MySalary::on_exportGradeButton_clicked()
{
    MySalary::saveToCSV("SELECT * FROM `grade`");
}

void MySalary::on_exportStaffButton_clicked()
{
    MySalary::saveToCSV("SELECT * FROM `staff`");
}

void MySalary::on_exportBonusButton_clicked()
{
    MySalary::saveToCSV("SELECT * FROM `bonus`");
}

void MySalary::on_exportSalaryButton_clicked()
{
    MySalary::saveToCSV("SELECT * FROM `salary`");
}

void MySalary::on_exportAdminButton_clicked()
{
    MySalary::saveToCSV("SELECT * FROM `admin`");
}
