#include <QDebug>

#include "extractor.h"

#include "invoke.h"

Invoke::Invoke(Extractor* extractor, QString const methodName, QJsonDocument const* in)
  : QObject(dynamic_cast<QObject*>(extractor))
  , extractor(extractor)
  , methodName(methodName)
  , in(*in)
{
}

QJsonDocument Invoke::run()
{
  char const* result;
  QJsonDocument document;

  result = invoke(extractor->thread, methodName.toLatin1().data(), in.toJson().data());
  QJsonParseError error;
  document = QJsonDocument::fromJson(QByteArray(result), &error);
  if (document.isNull()) {
    qDebug() << "JSON Parsing error: " << error.errorString();
  }

  this->deleteLater();
  return document;
}
