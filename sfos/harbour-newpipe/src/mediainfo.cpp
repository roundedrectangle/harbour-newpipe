#include "mediainfo.h"

MediaInfo::MediaInfo(QObject *parent)
  : QObject(parent)
  , name()
  , uploaderName()
  , category()
  , viewCount(0)
  , likeCount(0)
  , content()
{
}

MediaInfo::MediaInfo(QString const& url, QString const& name, QString const& uploaderName, QString const& category, int viewCount, int likeCount, QString const& content, QObject *parent)
  : QObject(parent)
  , url(url)
  , name(name)
  , uploaderName(uploaderName)
  , category(category)
  , viewCount(viewCount)
  , likeCount(likeCount)
  , content(content)
{

}

QString MediaInfo::getUrl() const
{
  return url;
}

QString MediaInfo::getName() const
{
  return name;
}

QString MediaInfo::getUploaderName() const
{
  return uploaderName;
}

QString MediaInfo::getCategory() const
{
  return category;
}

int MediaInfo::getViewCount() const
{
  return viewCount;
}

int MediaInfo::getLikeCount() const
{
  return likeCount;
}

QString MediaInfo::getContent() const
{
  return content;
}

void MediaInfo::setUrl(QString const& url)
{
  this->url = url;
}

void MediaInfo::setName(QString const& name)
{
  this->name = name;
}

void MediaInfo::setUploaderName(QString const& uploaderName)
{
  this->uploaderName = uploaderName;
}

void MediaInfo::setCategory(QString const& category)
{
  this->category = category;
}

void MediaInfo::setViewCount(int viewCount)
{
  this->viewCount = viewCount;
}

void MediaInfo::setLikeCount(int likeCount)
{
  this->likeCount = likeCount;
}

void MediaInfo::setContent(QString const& content)
{
  this->content = content;
}

