#include <QJsonObject>
#include <QDebug>

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

MediaInfo::MediaInfo(QString const& url, QString const& name, QString const& uploaderName, QString const& category, qint64 viewCount, qint64 likeCount, QString const& content, QObject *parent)
  : QObject(parent)
  , m_name(name)
  , m_uploaderName(uploaderName)
  , m_category(category)
  , m_viewCount(viewCount)
  , m_likeCount(likeCount)
  , m_content(content)
{

}

MediaInfo::MediaInfo(QJsonObject const& json, QObject *parent)
  : MediaInfo(parent)
{
  parseJsonChanges(json);
}

QString MediaInfo::name() const
{
  return m_name;
}

QString MediaInfo::uploaderName() const
{
  return m_uploaderName;
}

QString MediaInfo::category() const
{
  return m_category;
}

qint64 MediaInfo::viewCount() const
{
  return m_viewCount;
}

qint64 MediaInfo::likeCount() const
{
  return m_likeCount;
}

QString MediaInfo::content() const
{
  return m_content;
}

void MediaInfo::setName(QString const& name)
{
  if (m_name != name) {
    m_name = name;
    emit nameChanged();
  }
}

void MediaInfo::setUploaderName(QString const& uploaderName)
{
  if (m_uploaderName != uploaderName) {
    m_uploaderName = uploaderName;
    emit uploaderNameChanged();
  }
}

void MediaInfo::setCategory(QString const& category)
{
  if (m_category != category) {
    m_category = category;
    emit categoryChanged();
  }
}

void MediaInfo::setViewCount(qint64 viewCount)
{
  if (m_viewCount != viewCount) {
    m_viewCount = viewCount;
    emit viewCountChanged();
  }
}

void MediaInfo::setLikeCount(qint64 likeCount)
{
  if (m_likeCount != likeCount) {
    m_likeCount = likeCount;
    emit likeCountChanged();
  }
}

void MediaInfo::setContent(QString const& content)
{
  if (m_content != content) {
    m_content = content;
    emit contentChanged();
  }
}

void MediaInfo::parseJson(QJsonObject const& json)
{
  QList<MediaInfoSignal> emissions;
  emissions = parseJsonChanges(json);

  for (void (MediaInfo::*emission)() : emissions) {
    emit (this->*emission)();
  }
}

QList<MediaInfo::MediaInfoSignal> MediaInfo::parseJsonChanges(QJsonObject const& json)
{
  QList<MediaInfoSignal> emissions;
  QString name;
  QString uploaderName;
  QString category;
  int viewCount;
  int likeCount;
  QString content;

  name = json["name"].toString();
  if (m_name != name) {
    m_name = name;
    emissions << &MediaInfo::nameChanged;
  }
  uploaderName = json["uploaderName"].toString();
  if (m_uploaderName != uploaderName) {
    m_uploaderName = uploaderName;
    emissions << &MediaInfo::uploaderNameChanged;
  }
  category = json["category"].toString();
  if (m_category != category) {
    m_category = category;
    emissions << &MediaInfo::categoryChanged;
  }
  viewCount = json["likeCount"].toInt();
  if (m_viewCount != viewCount) {
    m_viewCount = viewCount;
    emissions << &MediaInfo::viewCountChanged;
  }
  likeCount = json["viewCount"].toInt();
  if (m_likeCount != likeCount) {
    m_likeCount = likeCount;
    emissions << &MediaInfo::likeCountChanged;
  }
  content = json["content"].toString();
  if (m_content != content) {
    m_content = content;
    emissions << &MediaInfo::contentChanged;
  }

  return emissions;
}



