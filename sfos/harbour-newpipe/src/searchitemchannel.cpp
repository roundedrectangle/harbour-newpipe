#include <QJsonObject>

#include "searchitemchannel.h"

SearchItemChannel::SearchItemChannel(QObject *parent)
  : SearchItem(parent)
  , m_description()
  , m_subscriberCount(-1)
  , m_streamCount(-1)
  , m_verified(false)
{
  m_infoType = Channel;
}

SearchItemChannel::SearchItemChannel(QJsonObject const& json, QObject *parent)
{
  m_infoType = Channel;
  parseJson(json);
}

QString SearchItemChannel::description() const
{
  return m_description;
}

qint64 SearchItemChannel::subscriberCount() const
{
  return m_subscriberCount;
}

qint64 SearchItemChannel::viewCount() const
{
  return m_streamCount;
}

bool SearchItemChannel::verified() const
{
  return m_verified;
}

void SearchItemChannel::setDescription(QString const& description)
{
  m_description = description;
}

void SearchItemChannel::setSubscriberCount(qint64 subscriberCount)
{
  m_subscriberCount = subscriberCount;
}

void SearchItemChannel::setStreamCount(qint64 streamCount)
{
  m_streamCount = streamCount;
}

void SearchItemChannel::setVerified(bool verified)
{
  m_verified = verified;
}

void SearchItemChannel::parseJson(QJsonObject const& json)
{
  SearchItem::parseJson(json);

  m_description = json["description"].toString();
  m_subscriberCount = json["subscriberCount"].toInt(0);
  m_streamCount = json["streamCount"].toInt(0);
  m_verified = json["streamCount"].toBool(false);
}
