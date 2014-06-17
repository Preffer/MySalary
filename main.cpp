#include "mysalary.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MySalary w;
    w.show();

    return a.exec();
}
