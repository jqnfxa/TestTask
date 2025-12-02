#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "qt/hull_controller.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<HullController>("com.task3.controllers", 1, 0, "HullController");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url] (QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
            {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection
    );
    engine.load(url);

    return app.exec();
}
