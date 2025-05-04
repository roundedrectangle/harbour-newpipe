#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <QObject>
#include <QFuture>

#include "appwrapper.h"

class Extractor : public QObject
{
  Q_OBJECT
public:
  explicit Extractor(QObject *parent = nullptr);
  ~Extractor();

public slots:
  void search();

private:
  QJsonDocument invokeSync(QString const methodName, QJsonDocument const* in);

  QFuture<QJsonDocument> invokeAsync(QString const methodName, QJsonDocument const* in);

private:
  graal_isolate_t* isolate;
  graal_isolatethread_t* thread;

signals:

};

#endif // EXTRACTOR_H
