#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class QQmlEngine;
class QJSEngine;

class Utils : public QObject
{
  Q_OBJECT
public:
  explicit Utils(QObject *parent = nullptr);
  static void instantiate(QObject* parent = nullptr);
  static Utils& getInstance();
  static QObject* provider(QQmlEngine* engine, QJSEngine* scriptEngine);
  Q_INVOKABLE static QString millisecondsToTime(quint32 milliseconds);

private:
  static Utils* instance;
};

#endif // UTILS_H
