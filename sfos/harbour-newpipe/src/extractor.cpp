#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <QtGlobal>
#include <QQmlEngine>

#include "invoke.h"
#include "searchmodel.h"
#include "commentmodel.h"
#include "mediainfo.h"
#include "pageref.h"
#include "lifetimecheck.h"
#include "channelinfo.h"
#include "channeltabinfo.h"
#include "listlinkhandler.h"

#include "extractor.h"

Extractor::Extractor(QObject *parent)
  : QObject(parent)
  , m_searchModel()
{
}

Extractor::Extractor(SearchModel* searchModel, QObject *parent)
  : QObject(parent)
  , m_searchModel(searchModel)
{
  QFuture<QString> initialise;
  m_threadPool.setExpiryTimeout(-1);
  m_threadPool.setMaxThreadCount(0);
  m_threadPool.reserveThread();

  initialise = QtConcurrent::run(&m_threadPool, [this]() {
    if (graal_create_isolate(NULL, &m_isolate, &m_thread) != 0) {
        fprintf(stderr, "initialization error\n");
    }
    init(m_thread);
    return QString();
  });
  initialise.waitForFinished();
}

Extractor::~Extractor()
{
  QFuture<QString> deinitialise;

  deinitialise = QtConcurrent::run(&m_threadPool, [this]() {
    char* result;
    result = invoke(m_thread, const_cast<char *>("tearDown"), const_cast<char *>("{}"));
    return QString(result);
  });
  deinitialise.waitForFinished();

  deinitialise = QtConcurrent::run(&m_threadPool, [this]() {
    graal_detach_thread(m_thread);
    return QString();
  });
  deinitialise.waitForFinished();

  m_threadPool.releaseThread();
}

QJsonDocument Extractor::invokeSync(QString const methodName, QJsonDocument const* in)
{
  Invoke* invoke = new Invoke(this, methodName, in);
  return invoke->run();
}

QFuture<QJsonDocument> Extractor::invokeAsync(QString const methodName, QJsonDocument const* in)
{
  Invoke* invoke = new Invoke(this, methodName, in);
  return QtConcurrent::run(&m_threadPool, invoke, &Invoke::run);
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
    QList<SearchItem const*> searchResults;
    for (QJsonValue const& item : items) {
      SearchItem const* deserialised = SearchItem::createSearchItem(item.toObject(), m_searchModel);
      searchResults.append(deserialised);
    }
    m_searchModel->replaceAll(searchResults);
    PageRef* page = new PageRef(result.object()["nextPage"].toObject(), m_searchModel);
    m_searchModel->setNextPage(page);

    delete watcher;
  });
  watcher->setFuture(invokeAsync("searchFor", &document));
}

void Extractor::searchMore(QString const& searchTerm, PageRef* page)
{
  QJsonObject json;
  QJsonArray contentFilter;
  QJsonDocument document;

  json["service"] = QStringLiteral("YouTube");
  json["searchString"] = searchTerm;
  contentFilter.push_back(QStringLiteral("all"));
  json["contentFilter"] = contentFilter;
  json["sortFilter"] = QStringLiteral("");
  json["page"] = page->toJson();
  document = QJsonDocument(json);

  QFutureWatcher<QJsonDocument>* watcher = new QFutureWatcher<QJsonDocument>();
  QObject::connect(watcher, &QFutureWatcher<QJsonDocument>::finished, [this, watcher]() {
    QJsonDocument result = watcher->result();
    //qDebug() << "Result: " << result.toJson(QJsonDocument::Indented);

    QJsonArray items = result.object()["itemsList"].toArray();
    QList<SearchItem const*> searchResults;
    for (QJsonValue const& item : items) {
      SearchItem const* deserialised = SearchItem::createSearchItem(item.toObject(), m_searchModel);
      searchResults.append(deserialised);
    }
    m_searchModel->append(searchResults);
    PageRef* page = new PageRef(result.object()["nextPage"].toObject(), m_searchModel);
    m_searchModel->setNextPage(page);

    delete watcher;
  });
  watcher->setFuture(invokeAsync("getMoreSearchItems", &document));
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

void Extractor::downloadExtract(MediaInfo* mediaInfo, QString const& url)
{
  QJsonObject json;
  QJsonDocument document;

  json["service"] = QStringLiteral("YouTube");
  json["url"] = url;
  document = QJsonDocument(json);

  QFutureWatcher<QJsonDocument>* watcher = new QFutureWatcher<QJsonDocument>();
  LifetimeCheck* lifetimeCheck = new LifetimeCheck(mediaInfo, watcher);
  QObject::connect(watcher, &QFutureWatcher<QJsonDocument>::finished, [this, watcher, url, mediaInfo, lifetimeCheck]() {
    if (!lifetimeCheck->destroyed()) {
      QJsonDocument result = watcher->result();
      //qDebug() << "Result: " << result.toJson(QJsonDocument::Indented);

      mediaInfo->parseJson(result.object());
    }

    delete watcher;
  });
  watcher->setFuture(invokeAsync("downloadExtract", &document));
}

void Extractor::getComments(CommentModel* commentModel, QString const& url)
{
  QJsonObject json;
  QJsonDocument document;

  json["service"] = QStringLiteral("YouTube");
  json["url"] = url;
  json["page"] = QJsonValue();
  document = QJsonDocument(json);

  QFutureWatcher<QJsonDocument>* watcher = new QFutureWatcher<QJsonDocument>();
  LifetimeCheck* lifetimeCheck = new LifetimeCheck(commentModel, watcher);
  QObject::connect(watcher, &QFutureWatcher<QJsonDocument>::finished, [this, watcher, commentModel, lifetimeCheck]() {
    if (!lifetimeCheck->destroyed()) {
      QJsonDocument result = watcher->result();
      //qDebug() << "Result: " << result.toJson(QJsonDocument::Indented);

      QJsonArray items = result.object()["relatedItems"].toArray();
      QList<CommentItem const*> comments;
      for (QJsonValue const& item : items) {
        CommentItem const* deserialised = new CommentItem(item.toObject(), commentModel);
        comments.append(deserialised);
      }
      commentModel->replaceAll(comments);
      PageRef* page = new PageRef(result.object()["nextPage"].toObject(), commentModel);
      commentModel->setNextPage(page);
    }

    delete watcher;
  });
  watcher->setFuture(invokeAsync("getCommentsInfo", &document));
}

void Extractor::getMoreComments(CommentModel* commentModel, QString const& url, PageRef* page)
{
  QJsonObject json;
  QJsonDocument document;

  json["service"] = QStringLiteral("YouTube");
  json["url"] = url;
  json["page"] = page->toJson();
  document = QJsonDocument(json);

  QFutureWatcher<QJsonDocument>* watcher = new QFutureWatcher<QJsonDocument>();
  LifetimeCheck* lifetimeCheck = new LifetimeCheck(commentModel, watcher);
  QObject::connect(watcher, &QFutureWatcher<QJsonDocument>::finished, [this, watcher, commentModel, lifetimeCheck]() {
    if (!lifetimeCheck->destroyed()) {
      QJsonDocument result = watcher->result();
      //qDebug() << "Result: " << result.toJson(QJsonDocument::Indented);

      QJsonArray items = result.object()["itemsList"].toArray();
      QList<CommentItem const*> comments;
      for (QJsonValue const& item : items) {
        CommentItem const* deserialised = new CommentItem(item.toObject(), commentModel);
        comments.append(deserialised);
      }
      commentModel->replaceAll(comments);
      PageRef* page = new PageRef(result.object()["nextPage"].toObject(), commentModel);
      commentModel->setNextPage(page);
    }
    delete watcher;
  });
  watcher->setFuture(invokeAsync("getMoreCommentItems", &document));
}

void Extractor::appendMoreComments(CommentModel* commentModel, QString const& url, PageRef* page)
{
  QJsonObject json;
  QJsonDocument document;

  json["service"] = QStringLiteral("YouTube");
  json["url"] = url;
  json["page"] = page->toJson();
  document = QJsonDocument(json);

  QFutureWatcher<QJsonDocument>* watcher = new QFutureWatcher<QJsonDocument>();
  LifetimeCheck* lifetimeCheck = new LifetimeCheck(commentModel, watcher);
  QObject::connect(watcher, &QFutureWatcher<QJsonDocument>::finished, [this, watcher, commentModel, lifetimeCheck]() {
    if (!lifetimeCheck->destroyed()) {
      QJsonDocument result = watcher->result();
      //qDebug() << "Result: " << result.toJson(QJsonDocument::Indented);

      QJsonArray items = result.object()["itemsList"].toArray();
      QList<CommentItem const*> comments;
      for (QJsonValue const& item : items) {
        CommentItem const* deserialised = new CommentItem(item.toObject(), commentModel);
        comments.append(deserialised);
      }
      commentModel->append(comments);
      PageRef* page = new PageRef(result.object()["nextPage"].toObject(), commentModel);
      commentModel->setNextPage(page);
    }
    delete watcher;
  });
  watcher->setFuture(invokeAsync("getMoreCommentItems", &document));
}

void Extractor::getChannelInfo(ChannelInfo* channelInfo, QString const& url)
{
  QJsonObject json;
  QJsonDocument document;

  json["service"] = QStringLiteral("YouTube");
  json["url"] = url;
  document = QJsonDocument(json);

  QFutureWatcher<QJsonDocument>* watcher = new QFutureWatcher<QJsonDocument>();
  LifetimeCheck* lifetimeCheck = new LifetimeCheck(channelInfo, watcher);
  QObject::connect(watcher, &QFutureWatcher<QJsonDocument>::finished, [this, watcher, channelInfo, lifetimeCheck]() {
    if (!lifetimeCheck->destroyed()) {
      QJsonDocument result = watcher->result();
      qDebug() << "Result: " << result.toJson(QJsonDocument::Indented);

      channelInfo->parseJson(result.object());
      emit extracted(channelInfo->url());
    }

    delete watcher;
  });
  watcher->setFuture(invokeAsync("getChannelInfo", &document));
}

void Extractor::getChannelTabInfo(ChannelTabInfo* channelTabInfo, ListLinkHandler const& linkHandler)
{
  QJsonObject json;
  QJsonDocument document;

  json["service"] = QStringLiteral("YouTube");
  json["linkHandler"] = linkHandler.toJson();
  document = QJsonDocument(json);

  QFutureWatcher<QJsonDocument>* watcher = new QFutureWatcher<QJsonDocument>();
  LifetimeCheck* lifetimeCheck = new LifetimeCheck(channelTabInfo, watcher);
  QObject::connect(watcher, &QFutureWatcher<QJsonDocument>::finished, [this, watcher, channelTabInfo, lifetimeCheck]() {
    if (!lifetimeCheck->destroyed()) {
      QJsonDocument result = watcher->result();
      qDebug() << "Result: " << result.toJson(QJsonDocument::Indented);

      channelTabInfo->parseJson(result.object());
    }

    delete watcher;
  });
  watcher->setFuture(invokeAsync("getChannelTabInfo", &document));
}
