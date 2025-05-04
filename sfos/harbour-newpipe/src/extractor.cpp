#include "extractor.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

Extractor::Extractor(QObject *parent) : QObject(parent)
{
//  if (graal_create_isolate(NULL, &isolate, &thread) != 0) {
//      fprintf(stderr, "initialization error\n");
//  }
//  init(thread);
}

Extractor::~Extractor()
{
  char* result;

  qDebug() << "Tearing down";
  //result = invoke(thread, const_cast<char *>("tearDown"), const_cast<char *>("{}"));
  qDebug() << "Teardown result: " << result;

  qDebug() << "Detaching thread";
  //graal_detach_thread(thread);
}

QJsonDocument Extractor::invokeSync(QString const methodName, QJsonDocument const* in)
{
  char const* result;
  QJsonObject json;
  QJsonArray contentFilter;
  QJsonDocument document;

  json["service"] = QStringLiteral("YouTube");
  json["searchString"] = QStringLiteral("testing");
  contentFilter.push_back(QStringLiteral("all"));
  json["contentFilter"] = contentFilter;
  json["sortFilter"] = QStringLiteral("");
  document = QJsonDocument(json);

  qDebug() << "JSON input: " << document.toJson().data();
  result = invoke(thread, (char *)"searchFor", document.toJson().data());

  QJsonParseError error;
  document = QJsonDocument::fromJson(QByteArray(result), &error);
  if (document.isNull()) {
    qDebug() << "JSON Parsing error: " << error.errorString();
  }

  return document;
}

QFuture<QJsonDocument> Extractor::invokeAsync(QString const methodName, QJsonDocument const* in)
{
  return QtConcurrent::run(QThreadPool::globalInstance(), [this, methodName, in]() {
    if (graal_create_isolate(NULL, &isolate, &thread) != 0) {
        fprintf(stderr, "initialization error\n");
    }
    init(thread);
    QJsonDocument result = invokeSync(QString("searchFor"), in);
    graal_detach_thread(thread);

    return result;
  });
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
