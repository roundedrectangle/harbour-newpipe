#include <QJsonObject>
#include <QDateTime>

#include "searchitemstream.h"

SearchItemStream::SearchItemStream(QObject *parent)
  : SearchItem(parent)
  , m_uploaderName()
  , m_uploadDate(0)
{
  m_infoType = Stream;
}

SearchItemStream::SearchItemStream(QJsonObject const& json, QObject *parent)
  : SearchItem(parent)
{
  m_infoType = Stream;
  parseJson(json);
}

QString SearchItemStream::uploaderName() const
{
  return m_uploaderName;
}

quint64 SearchItemStream::uploadDate() const
{
  return m_uploadDate;
}

void SearchItemStream::setUploaderName(QString const& uploaderName)
{
  m_uploaderName = uploaderName;
}

void SearchItemStream::setUploadDate(quint64 uploadDate)
{
  m_uploadDate = uploadDate;
}

void SearchItemStream::parseJson(QJsonObject const& json)
{
  SearchItem::parseJson(json);

  m_uploaderName = json["uploaderName"].toString();
  m_uploadDate = json["uploadDate"].toObject()["offsetDateTime"].toInt(0);
}

QString SearchItemStream::getInfoRow() const
{
  QDateTime dateTIme = QDateTime::fromMSecsSinceEpoch(m_uploadDate);

  return QString("%1:%2:%3").arg(hours).arg(minutes, 2, 'f', 0, '0').arg(seconds, 2, 'f', 0, '0');
}
