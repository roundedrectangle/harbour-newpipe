#include "extractor.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

class Invoke : public QObject {
public:
  Invoke(Extractor* extractor, QString const methodName, QJsonDocument const* in)
    : QObject(dynamic_cast<QObject*>(extractor))
    , extractor(extractor)
    , methodName(methodName)
    , in(*in)
  {
  }

  QJsonDocument run()
  {
    char const* result;
    QJsonDocument document;

    qDebug() << "JSON input: " << in.toJson().data();
    result = invoke(extractor->thread, methodName.toLatin1().data(), in.toJson().data());
    QJsonParseError error;
    document = QJsonDocument::fromJson(QByteArray(result), &error);
    if (document.isNull()) {
      qDebug() << "JSON Parsing error: " << error.errorString();
    }
    qDebug() << "JSON output: " << document.toJson().data();

    this->deleteLater();
    return document;
  }
private:
  Extractor const* extractor;
  QString methodName;
  QJsonDocument in;
};

Extractor::Extractor(QObject *parent) : QObject(parent)
{
  QFuture<QString> initialise;
  threadPool.setMaxThreadCount(1);

  initialise = QtConcurrent::run(&threadPool, [this]() {
    if (graal_create_isolate(NULL, &isolate, &thread) != 0) {
        fprintf(stderr, "initialization error\n");
    }
    init(thread);
    return QString();
  });
  initialise.waitForFinished();
}

Extractor::~Extractor()
{
  QFuture<QString> deinitialise;

  deinitialise = QtConcurrent::run(&threadPool, [this]() {
    char* result;
    result = invoke(thread, const_cast<char *>("tearDown"), const_cast<char *>("{}"));
    return QString(result);
  });
  deinitialise.waitForFinished();

  deinitialise = QtConcurrent::run(&threadPool, [this]() {
    graal_detach_thread(thread);
    return QString();
  });
  deinitialise.waitForFinished();
}

QJsonDocument Extractor::invokeSync(QString const methodName, QJsonDocument const* in)
{
  Invoke* invoke = new Invoke(this, methodName, in);
  return invoke->run();
}

QFuture<QJsonDocument> Extractor::invokeAsync(QString const methodName, QJsonDocument const* in)
{
  Invoke* invoke = new Invoke(this, methodName, in);
  return QtConcurrent::run(&threadPool, invoke, &Invoke::run);
}

void Extractor::search()
{
  QJsonObject json;
  QJsonArray contentFilter;
  QJsonDocument document;

  json["service"] = QStringLiteral("YouTube");
  json["searchString"] = QStringLiteral("testing");
  contentFilter.push_back(QStringLiteral("all"));
  json["contentFilter"] = contentFilter;
  json["sortFilter"] = QStringLiteral("");
  document = QJsonDocument(json);

  qDebug() << "Search input: " << json;

  QFutureWatcher<QJsonDocument>* watcher = new QFutureWatcher<QJsonDocument>();
  QObject::connect(watcher, &QFutureWatcher<QJsonDocument>::finished, [watcher]() {
    qDebug() << "Search result: " << watcher->result();
    delete watcher;
  });
  watcher->setFuture(invokeAsync("searchFor", &document));
}
