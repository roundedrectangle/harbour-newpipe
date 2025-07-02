#include <QJsonObject>
#include <QJsonArray>

#include "listlinkhandler.h"

#include "channelinfo.h"

ChannelInfo::ChannelInfo(QObject *parent)
  : QObject(parent)
  , m_id()
  , m_name()
  , m_url()
  , m_description()
  , m_tabs()
{

}

ChannelInfo::ChannelInfo(QString const& id, QString const& name, QString const& url, QString const& description, QList<ListLinkHandler const*> const& tabs, QObject *parent)
  : QObject(parent)
  , m_id(id)
  , m_name(name)
  , m_url(url)
  , m_description(description)
  , m_tabs(tabs)
{

}

ChannelInfo::ChannelInfo(QJsonObject const& json, QObject *parent)
  : ChannelInfo(parent)
{
  parseJson(json);
}

QString ChannelInfo::id() const
{
  return m_id;
}

QString ChannelInfo::name() const
{
  return m_name;
}

QString ChannelInfo::url() const
{
  return m_url;
}

QString ChannelInfo::description() const
{
  return m_description;
}

QList<ListLinkHandler const*> ChannelInfo::tabs()
{
  return m_tabs;
}

void ChannelInfo::setId(QString const& id)
{
  m_id = id;
}

void ChannelInfo::setName(QString const& name)
{
  m_name = name;
}

void ChannelInfo::setUrl(QString const& url)
{
  m_url = url;
}

void ChannelInfo::setDescription(QString const& description)
{
  m_description = description;
}

void ChannelInfo::setTabs(QList<ListLinkHandler const*> const& tabs)
{
  m_tabs = tabs;
}

void ChannelInfo::parseJson(QJsonObject const& json)
{
  m_id = json["id"].toString();
  m_name = json["name"].toString();
  m_url = json["url"].toString();
  m_description = json["description"].toString();

  QJsonArray tabs = json["tabs"].toArray();
  m_tabs.clear();
  for (QJsonValue const& tabItem : tabs) {
    ListLinkHandler* tab = new ListLinkHandler(tabItem.toObject(), this);
    m_tabs.append(tab);
  }
}
