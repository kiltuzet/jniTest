#include "mainwindow.h"
#include <QJniObject>
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QJniObject::callStaticMethod<void>(
        "org/example/da/HelloEpta",
        "hello",
        "()V");
    QJniObject arg = QJniObject::fromString("ogre");
    QJniObject result = QJniObject::callStaticObjectMethod(
        "org/example/da/HelloEpta",
        "retres",
        "(Ljava/lang/String;)Ljava/lang/String;",
        arg.object<jstring>()
        );
    qDebug()<<result.toString();
    MainWindow w;
    w.show();
    return a.exec();
}
