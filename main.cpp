#include <QJniObject>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1) Статический void-метод без параметров: ()V
    QJniObject::callStaticMethod<void>(
        "org/example/da/HelloEpta",
        "hello",
        "()V"
        );

    // 2) Статический метод, принимающий String и возвращающий String:
    QJniObject arg = QJniObject::fromString("ogre");
    QJniObject result = QJniObject::callStaticObjectMethod(
        "org/example/da/HelloEpta",
        "retres",
        "(Ljava/lang/String;)Ljava/lang/String;",
        arg.object<jstring>()
        );


    qDebug() << "Result from Java:" << result.toString();

    // Статический void-метод без параметров: ()V
    QJniObject::callStaticMethod<void>(
        "org/example/da/HelloKotlin",
        "f1",
        "()V"
        );

    // Статический метод: принимает String, возвращает String
    QJniObject jArg = QJniObject::fromString("ogre");
    QJniObject jRes = QJniObject::callStaticObjectMethod(
        "org/example/da/HelloKotlin",
        "f2",
        "(Ljava/lang/String;)Ljava/lang/String;",
        jArg.object<jstring>()
        );
    qDebug() << "Kotlin says:" << jRes.toString();

    return a.exec();
}
