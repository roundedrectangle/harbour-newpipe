#include <QJsonObject>

#include "searchitemplaylist.h"

SearchItemPlaylist::SearchItemPlaylist(QObject *parent)
  : SearchItem(parent)
  , m_uploaderName()
  , m_uploaderVerified(false)
  , m_streamCount(-1)
  , m_description()
{
  m_infoType = Playlist;
}

SearchItemPlaylist::SearchItemPlaylist(QJsonObject const& json, QObject *parent)
  : SearchItem(parent)
{
  m_infoType = Playlist;
  parseJson(json);
}

QString SearchItemPlaylist::uploaderName() const
{
  return m_uploaderName;
}

bool SearchItemPlaylist::uploaderVerified() const
{
  return m_uploaderVerified;
}

qint64 SearchItemPlaylist::streamCount() const
{
  return m_streamCount;
}

QString SearchItemPlaylist::description() const
{
  return m_description;
}

void SearchItemPlaylist::setUploaderName(QString const& uploaderName)
{
  m_uploaderName = uploaderName;
}

void SearchItemPlaylist::setUploaderVerified(bool uploaderVerified)
{
  m_uploaderVerified = uploaderVerified;
}

void SearchItemPlaylist::setStreamCount(qint64 streamCount)
{
  m_streamCount = streamCount;
}

void SearchItemPlaylist::setDescription(QString const& description)
{
  m_description = description;
}

void SearchItemPlaylist::parseJson(QJsonObject const& json)
{
  SearchItem::parseJson(json);

  m_uploaderName = json["uploaderName"].toString();
  m_uploaderVerified = json["uploaderVerified"].toBool(false);
  m_streamCount = json["streamCount"].toInt(-1);
  m_description = json["description"].toString();
}
