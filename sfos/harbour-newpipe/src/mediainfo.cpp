#include <QJsonObject>

#include "mediainfo.h"

MediaInfo::MediaInfo(QObject *parent)
  : QObject(parent)
  , m_name()
  , m_uploaderName()
  , m_category()
  , m_viewCount(0)
  , m_likeCount(0)
  , m_content()
{
}

MediaInfo::MediaInfo(QString const& url, QString const& name, QString const& uploaderName, QString const& category, int viewCount, int likeCount, QString const& content, QObject *parent)
  : QObject(parent)
  , m_url(url)
  , m_name(name)
  , m_uploaderName(uploaderName)
  , m_category(category)
  , m_viewCount(viewCount)
  , m_likeCount(likeCount)
  , m_content(content)
{

}

MediaInfo::MediaInfo(QJsonObject const& json, QObject *parent)
  : QObject(parent)
{
  parseJson(json);
}

QString MediaInfo::getUrl() const
{
  return m_url;
}

QString MediaInfo::getName() const
{
  return m_name;
}

QString MediaInfo::getUploaderName() const
{
  return m_uploaderName;
}

QString MediaInfo::getCategory() const
{
  return m_category;
}

int MediaInfo::getViewCount() const
{
  return m_viewCount;
}

int MediaInfo::getLikeCount() const
{
  return m_likeCount;
}

QString MediaInfo::getContent() const
{
  return m_content;
}

void MediaInfo::setUrl(QString const& url)
{
  m_url = url;
}

void MediaInfo::setName(QString const& name)
{
  m_name = name;
}

void MediaInfo::setUploaderName(QString const& uploaderName)
{
  m_uploaderName = uploaderName;
}

void MediaInfo::setCategory(QString const& category)
{
  m_category = category;
}

void MediaInfo::setViewCount(int viewCount)
{
  m_viewCount = viewCount;
}

void MediaInfo::setLikeCount(int likeCount)
{
  m_likeCount = likeCount;
}

void MediaInfo::setContent(QString const& content)
{
  m_content = content;
}

void MediaInfo::parseJson(QJsonObject const& json)
{
  m_name = json["name"].toString();
  m_uploaderName = json["uploaderName"].toString();
  m_category = json["category"].toString();
  m_viewCount = json["likeCount"].toInt();
  m_likeCount = json["viewCount"].toInt();
  m_content = json["content"].toString();
}



