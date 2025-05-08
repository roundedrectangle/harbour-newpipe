#include "searchitem.h"

SearchItem::SearchItem()
  : name()
  , thumbnail()
{
}

SearchItem::SearchItem(QString const& name, QString const& thumbnail, QString const& url)
  : name(name)
  , thumbnail(thumbnail)
  , url(url)
{
}

QString SearchItem::getName() const
{
  return name;
}

QString SearchItem::getThumbnail() const
{
  return thumbnail;
}

QString SearchItem::getUrl() const
{
  return url;
}

void SearchItem::setName(QString const& name)
{
  this->name = name;
}

void SearchItem::setThumbnail(QString const& thumbnail)
{
  this->thumbnail = thumbnail;
}

void SearchItem::setUrl(QString const& url)
{
  this->url = url;
}
