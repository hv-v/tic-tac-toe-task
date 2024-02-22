#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "helpers.h"
#include "models/GameModel.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/tic-tac-toe/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
    Qt::QueuedConnection);

    tic_tac_toe::GameModel rootGameModel;
    engine.rootContext()->setContextProperty("gameModel", &rootGameModel);

    qmlRegisterUncreatableMetaObject(
        tic_tac_toe::helpers::staticMetaObject,
        "com.tic_tac_toe.enums", 1, 0, "PlayerHelpers", "Use for enums only" );

    qmlRegisterUncreatableMetaObject(
        tic_tac_toe::helpers::error_handler::staticMetaObject,
        "com.tic_tac_toe.error_types", 1, 0, "ErrorType", "Use for enums only" );

    engine.load(url);

    return app.exec();
}
