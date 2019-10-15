#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "game.h"

int main(int argc, char *argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  qmlRegisterType<bowling::Game>("Game", 1, 0, "Game");
  bowling::Game game;

  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("game", &game);

  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
      &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      }, Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
