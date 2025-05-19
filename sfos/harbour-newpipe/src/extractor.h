#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <QObject>
#include <QFuture>
#include <QThreadPool>

#include "appwrapper.h"

class SearchModel;
class CommentModel;
class MediaInfo;

class Extractor : public QObject
{
  Q_OBJECT
public:
  explicit Extractor(QObject *parent = nullptr);
  explicit Extractor(SearchModel* searchModel, CommentModel* commentModel, QObject *parent = nullptr);
  ~Extractor();

  static int compareResolutions(QString const& first, QString const& second);

public slots:
  void search(QString const& searchTerm);
  void downloadExtract(QString const& url);
  void getComments(QString const& url);
  MediaInfo const* getMediaInfo(QString const& url) const;

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
  CommentModel* m_commentModel;
  QMap<QString, MediaInfo const*> m_mediaInfo;
};

#endif // EXTRACTOR_H
