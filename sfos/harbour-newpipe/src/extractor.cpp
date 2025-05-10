#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <QtGlobal>

#include "invoke.h"
#include "searchmodel.h"
#include "mediainfo.h"

#include "extractor.h"

Extractor::Extractor(QObject *parent)
  : QObject(parent)
  , searchModel()
{
}

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
    //qDebug() << "Result: " << result.toJson(QJsonDocument::Indented);

    QJsonArray items = result.object()["relatedItems"].toArray();
    QList<SearchItem> searchResults;
    for (QJsonValue const& item : items) {
      QJsonObject entry = item.toObject();
      //qDebug() << "Entry: " << entry["name"].toString();

      QString thumbnailUrl;
      QJsonArray thumbnails = entry["thumbnails"].toArray();
      QString resolutionLevel;
      for (QJsonValue const& thumbnail : thumbnails) {
        QJsonObject details = thumbnail.toObject();
        QString estimatedResolutionLevel = details["estimatedResolutionLevel"].toString();
        if (resolutionLevel.isEmpty() || compareResolutions(resolutionLevel, estimatedResolutionLevel) < 0) {
          thumbnailUrl = details["url"].toString();
          resolutionLevel = estimatedResolutionLevel;
        }
      }
      QString name = entry["name"].toString();
      QString url = entry["url"].toString();
      SearchItem result(name, thumbnailUrl, url);
      searchResults.append(result);
    }
    this->searchModel->replaceAll(searchResults);

    delete watcher;
  });
  watcher->setFuture(invokeAsync("searchFor", &document));
}

int Extractor::compareResolutions(QString const& first, QString const& second)
{
  static QStringList const ordering = {"", "HIGH", "MEDIUM", "LOW"};
  int result = 0;
  int firstIndex;
  int secondIndex;

  firstIndex = ordering.indexOf(first);
  secondIndex = ordering.indexOf(second);
  result = qBound(-1, firstIndex - secondIndex, 1);

  return result;
}

void Extractor::downloadExtract(QString const& url)
{
  QJsonObject json;
  QJsonDocument document;

  json["service"] = QStringLiteral("YouTube");
  json["url"] = url;
  document = QJsonDocument(json);

  QFutureWatcher<QJsonDocument>* watcher = new QFutureWatcher<QJsonDocument>();
  QObject::connect(watcher, &QFutureWatcher<QJsonDocument>::finished, [this, watcher, url]() {
    QJsonDocument result = watcher->result();
    //qDebug() << "Result: " << result.toJson(QJsonDocument::Indented);
    QJsonObject root = result.object();
    QString name = root["name"].toString();
    QString uploaderName = root["uploaderName"].toString();
    QString category = root["category"].toString();
    int viewCount = root["likeCount"].toInt();
    int likeCount = root["viewCount"].toInt();
    QString content = root["content"].toString();

    qDebug() << "Name: " << name;
    qDebug() << "Uploader name: " << uploaderName;
    qDebug() << "Category: " << category;
    qDebug() << "View Count: " << viewCount;
    qDebug() << "Like Count: " << likeCount;
    qDebug() << "Content: " << content;

    MediaInfo* info = new MediaInfo(url, name, uploaderName, category, viewCount, likeCount, content, this);
    mediaInfo.insert(url, info);

    emit extracted(url);

    delete watcher;
  });
  watcher->setFuture(invokeAsync("downloadExtract", &document));
}

MediaInfo* Extractor::getMediaInfo(QString const& url) const
{
  static MediaInfo nomedia;
  return mediaInfo.value(url, &nomedia);
}
