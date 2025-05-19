#include <QJsonObject>
#include <QJsonArray>

#include "extractor.h"

#include "searchitem.h"

SearchItem::SearchItem(QObject *parent)
  : QObject(parent)
  , m_name()
  , m_thumbnail()
{
}

SearchItem::SearchItem(QString const& name, QString const& thumbnail, QString const& url, QObject *parent)
  : QObject(parent)
  , m_name(name)
  , m_thumbnail(thumbnail)
  , m_url(url)
{
}

SearchItem::SearchItem(QJsonObject const& json, QObject *parent)
  : QObject(parent)
{
  parseJson(json);
}

QString SearchItem::getName() const
{
  return m_name;
}

QString SearchItem::getThumbnail() const
{
  return m_thumbnail;
}

QString SearchItem::getUrl() const
{
  return m_url;
}

void SearchItem::setName(QString const& name)
{
  m_name = name;
}

void SearchItem::setThumbnail(QString const& thumbnail)
{
  m_thumbnail = thumbnail;
}

void SearchItem::setUrl(QString const& url)
{
  m_url = url;
}

void SearchItem::parseJson(QJsonObject const& json)
{
  QString thumbnailUrl;
  QJsonArray thumbnails = json["thumbnails"].toArray();
  QString resolutionLevel;
  for (QJsonValue const& thumbnail : thumbnails) {
    QJsonObject details = thumbnail.toObject();
    QString estimatedResolutionLevel = details["estimatedResolutionLevel"].toString();
    if (resolutionLevel.isEmpty() || Extractor::compareResolutions(resolutionLevel, estimatedResolutionLevel) < 0) {
      thumbnailUrl = details["url"].toString();
      resolutionLevel = estimatedResolutionLevel;
    }
  }
  m_name = json["name"].toString();
  m_thumbnail = thumbnailUrl;
  m_url = json["url"].toString();
}
