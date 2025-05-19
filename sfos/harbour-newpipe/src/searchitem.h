#ifndef SEARCHITEM_H
#define SEARCHITEM_H

#include <QObject>
#include <QString>

class SearchItem : QObject {
  Q_OBJECT
public:
  SearchItem(QObject *parent = nullptr);
  explicit SearchItem(QString const& name, QString const& thumbnail, QString const& url, QObject *parent = nullptr);
  explicit SearchItem(QJsonObject const& json, QObject *parent = nullptr);

  QString getName() const;
  QString getThumbnail() const;
  QString getUrl() const;

  void setName(QString const& name);
  void setThumbnail(QString const& thumbnail);
  void setUrl(QString const& url);

  void parseJson(QJsonObject const& json);

private:
  QString m_name;
  QString m_thumbnail;
  QString m_url;
};

#endif // SEARCHITEM_H
