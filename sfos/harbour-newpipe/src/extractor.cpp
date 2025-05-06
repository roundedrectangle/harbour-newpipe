#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

#include "invoke.h"
#include "searchmodel.h"

#include "extractor.h"

Extractor::Extractor(SearchModel* searchModel, QObject *parent)
  : QObject(parent)
  , searchModel(searchModel)
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

void Extractor::search(QString const& searchTerm)
{
  QJsonObject json;
  QJsonArray contentFilter;
  QJsonDocument document;

  json["service"] = QStringLiteral("YouTube");
  json["searchString"] = searchTerm;
  contentFilter.push_back(QStringLiteral("all"));
  json["contentFilter"] = contentFilter;
  json["sortFilter"] = QStringLiteral("");
  document = QJsonDocument(json);

  QFutureWatcher<QJsonDocument>* watcher = new QFutureWatcher<QJsonDocument>();
  QObject::connect(watcher, &QFutureWatcher<QJsonDocument>::finished, [this, watcher]() {
    QJsonDocument result = watcher->result();

    QJsonArray items = result.object()["relatedItems"].toArray();
    QStringList searchResults;
    for (QJsonValue const& item : items) {
      QJsonObject entry = item.toObject();
      qDebug() << "Entry: " << entry["name"].toString();
      searchResults.append(entry["name"].toString());
    }
    this->searchModel->replaceAll(searchResults);

    delete watcher;
  });
  watcher->setFuture(invokeAsync("searchFor", &document));
}
