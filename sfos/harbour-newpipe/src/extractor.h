#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <QObject>
#include <QFuture>
#include <QThreadPool>

#include "appwrapper.h"

class SearchModel;

class Extractor : public QObject
{
  Q_OBJECT
public:
  explicit Extractor(SearchModel* searchModel, QObject *parent = nullptr);
  ~Extractor();

public slots:
  void search(QString const& searchTerm);

private:
  QJsonDocument invokeSync(QString const methodName, QJsonDocument const* in);

  QFuture<QJsonDocument> invokeAsync(QString const methodName, QJsonDocument const* in);

  static int compareResolutions(QString const& first, QString const& second);

public:
  graal_isolate_t* isolate;
  graal_isolatethread_t* thread;
  QThreadPool threadPool;
  SearchModel* searchModel;

signals:

};

#endif // EXTRACTOR_H
