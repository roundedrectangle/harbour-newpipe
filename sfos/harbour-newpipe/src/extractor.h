#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <QObject>
#include <QFuture>
#include <QThreadPool>

#include "appwrapper.h"

class SearchModel;
class CommentModel;
class MediaInfo;
class PageRef;
class ChannelInfo;
class ChannelTabInfo;
class ListLinkHandler;
class LinkHandlerModel;

class Extractor : public QObject
{
  Q_OBJECT
public:
  explicit Extractor(QObject *parent = nullptr);
  explicit Extractor(SearchModel* searchModel, QObject *parent = nullptr);
  ~Extractor();

  static int compareResolutions(QString const& first, QString const& second);

public slots:
  void search(QString const& searchTerm);
  void searchMore(QString const& searchTerm, PageRef* page);
  void downloadExtract(MediaInfo* mediaInfo, QString const& url);
  void getComments(CommentModel* commentModel, QString const& url);
  void getMoreComments(CommentModel* commentModel, QString const& url, PageRef* page);
  void appendMoreComments(CommentModel* commentModel, QString const& url, PageRef* page);
  void getChannelInfo(ChannelInfo* channelInfo, LinkHandlerModel* linkHandlerModel, QString const& url);
  void getChannelTabInfo(ChannelTabInfo* channelTabInfo, ListLinkHandler* linkHandler);

signals:
  void extracted(QString const& url);

private:
  QJsonDocument invokeSync(QString const methodName, QJsonDocument const* in);

  QFuture<QJsonDocument> invokeAsync(QString const methodName, QJsonDocument const* in);

public:
  graal_isolate_t* m_isolate;
  graal_isolatethread_t* m_thread;
  QThreadPool m_threadPool;
  SearchModel* m_searchModel;
};

#endif // EXTRACTOR_H
