#ifndef PAGEREF_H
#define PAGEREF_H

#include <QObject>
#include <QMap>

class PageRef : public QObject
{
  Q_OBJECT
public:
  explicit PageRef(QObject *parent = nullptr);
  explicit PageRef(QString const& url, QString const& id, QStringList const& ids, QMap<QString, QString> const& cookies, QString const& body, QObject *parent = nullptr);
  explicit PageRef(QJsonObject const& json, QObject *parent = nullptr);

  QString getUrl() const;
  QString getId() const;
  QStringList getIds() const;
  QMap<QString, QString> getCookies(QString const& key) const;
  QString getBody();

  void setUrl(QString const& url);
  void setId(QString const& id);
  void setIds(QStringList const& ids);
  void setCookies(QMap<QString, QString> const& cookies);
  void setBody(QString const& body);

  void parseJson(QJsonObject const& json);
  QJsonObject toJson() const;

signals:

private:
  QString m_url;
  QString m_id;
  QStringList m_ids;
  QMap<QString, QString> m_cookies;
  QString m_body;
};

#endif // PAGEREF_H
