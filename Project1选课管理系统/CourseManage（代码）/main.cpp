#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("://image/1.png"));
    LoginDialog w;
    w.show();

    return a.exec();
}
