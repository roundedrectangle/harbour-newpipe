#include <QJsonObject>
#include <QJsonArray>

#include "page.h"

Page::Page(QObject *parent)
  : QObject(parent)
  , m_url()
  , m_id()
  , m_ids()
  , m_cookies()
  , m_body()
{
}

Page::Page(QString const& url, QString const& id, QStringList const& ids, QMap<QString, QString> const& cookies, QString const& body, QObject *parent)
  : QObject(parent)
  , m_url(url)
  , m_id(id)
  , m_ids(ids)
  , m_cookies(cookies)
  , m_body(body)
{
}

Page::Page(QJsonObject const& json, QObject *parent)
  : QObject(parent)
{
  parseJson(json);
}

QString Page::getUrl() const
{
  return m_url;
}

QString Page::getId() const
{
  return m_id;
}

QStringList Page::getIds() const
{
  return m_ids;
}

QMap<QString, QString> Page::getCookies(QString const& key) const
{
  return m_cookies;
}

QString Page::getBody()
{
  return m_body;
}

void Page::setUrl(QString const& url)
{
  m_url = url;
}

void Page::setId(QString const& id)
{
  m_id = id;
}

void Page::setIds(QStringList const& ids)
{
  m_ids = ids;
}

void Page::setCookies(QMap<QString, QString> const& cookies)
{
  m_cookies = cookies;
}

void Page::setBody(QString const& body)
{
  m_body = body;
}

void Page::parseJson(QJsonObject const& json)
{
  m_url = json["url"].toString();
  m_id = json["id"].toString();
  m_ids.clear();
  if (json["ids"].isArray()) {
    for (QJsonValue const& id : json["ids"].toArray()) {
      m_ids.append(id.toString());
    }
  }
  m_cookies.clear();
  if (json["cookies"].isObject()) {
    QJsonObject const cookies = json["cookies"].toObject();
    QJsonObject::const_iterator cookie = cookies.constBegin();
    while (cookie != cookies.end()) {
      m_cookies.insert(cookie.key(), cookie.value().toString());
      ++cookie;
    }
  }
  m_body = QString();
  if (json["body"].isString()) {
    m_body = json["body"].toString();
  }
}
