#include "mysalary.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MySalary w;
    QFont sans("Microsoft YaHei");
    w.setFont(sans);
    w.show();

    return a.exec();
}
