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

public slots:
  void search(QString const& searchTerm);
  void downloadExtract(QString const& url);
  void getComments(QString const& url);
  MediaInfo* getMediaInfo(QString const& url) const;

signals:
  void extracted(QString const& url);

private:
  QJsonDocument invokeSync(QString const methodName, QJsonDocument const* in);

  QFuture<QJsonDocument> invokeAsync(QString const methodName, QJsonDocument const* in);

  static int compareResolutions(QString const& first, QString const& second);

public:
  graal_isolate_t* isolate;
  graal_isolatethread_t* thread;
  QThreadPool threadPool;
  SearchModel* searchModel;
  CommentModel* commentModel;
  QMap<QString, MediaInfo*> mediaInfo;
};

#endif // EXTRACTOR_H
