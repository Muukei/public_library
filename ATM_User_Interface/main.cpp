#include "LoginDialog.h"
#include <QFile>
#include <QApplication>
#include <LoginDialog.h>
#include<QInputMethod>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    LoginDialog w;
    QFile styleFile(":/css/default.css");
    styleFile.open(QFile::ReadOnly);
    QString styleSheet = styleFile.readAll();
    a.setStyleSheet(styleSheet);
    w.show();
    return a.exec();
}
