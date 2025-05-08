#include "searchitem.h"

SearchItem::SearchItem()
  : name()
  , thumbnail()
{
}

SearchItem::SearchItem(QString const& name, QString const& thumbnail)
  : name(name)
  , thumbnail(thumbnail)
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

void SearchItem::setName(QString const& name)
{
  this->name = name;
}

void SearchItem::setThumbnail(QString const& thumbnail)
{
  this->thumbnail = thumbnail;
}
