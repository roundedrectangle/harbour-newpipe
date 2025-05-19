#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "extractor.h"
#include "page.h"

#include "commentitem.h"

CommentItem::CommentItem(QObject *parent)
  : QObject(parent)
  , m_commentText()
  , m_uploaderName()
  , m_uploaderAvatar()
  , m_replyCount(0)
  , m_page()
{

}

CommentItem::CommentItem(QString const& commentText, QString const& uploaderName, QString const& uploaderAvatar, qint64 replyCount, Page* page, QObject *parent)
  : QObject(parent)
  , m_commentText(commentText)
  , m_uploaderName(uploaderName)
  , m_uploaderAvatar(uploaderAvatar)
  , m_replyCount(replyCount)
  , m_page(page)
{
}

CommentItem::CommentItem(QJsonObject const& json, QObject *parent )
  : QObject(parent)
{
  parseJson(json);
}

QString CommentItem::getCommentText() const
{
  return m_commentText;
}

QString CommentItem::getUploaderName() const
{
  return m_uploaderName;
}

QString CommentItem::getUploaderAvatar() const
{
  return m_uploaderAvatar;
}

qint64 CommentItem::getReplyCount() const
{
  return m_replyCount;
}

Page* CommentItem::getPage() const
{
  return m_page;
}

void CommentItem::setCommentText(QString const& commentText)
{
  m_commentText = commentText;
}

void CommentItem::setUploaderName(QString const& uploaderName)
{
  m_uploaderName = uploaderName;
}

void CommentItem::setUploaderAvatar(QString const& uploaderAvatar)
{
  m_uploaderAvatar = uploaderAvatar;
}

void CommentItem::setReplyCount(qint64 replyCount)
{
  m_replyCount = replyCount;
}

void CommentItem::setPage(Page* page)
{
  m_page = page;
}

void CommentItem::parseJson(QJsonObject const& json)
{
  QString uploaderAvatarUrl;
  QJsonArray uploaderAvatars = json["uploaderAvatars"].toArray();
  QString resolutionLevel;
  for (QJsonValue const& uploaderAvatar : uploaderAvatars) {
    QJsonObject details = uploaderAvatar.toObject();
    QString estimatedResolutionLevel = details["estimatedResolutionLevel"].toString();
    if (resolutionLevel.isEmpty() || Extractor::compareResolutions(resolutionLevel, estimatedResolutionLevel) < 0) {
      uploaderAvatarUrl = details["url"].toString();
      resolutionLevel = estimatedResolutionLevel;
    }
  }
  m_commentText = json["commentText"].toObject()["content"].toString();
  m_uploaderName = json["uploaderName"].toString();
  m_uploaderAvatar = uploaderAvatarUrl;
  m_replyCount = json["replyCount"].toInt(0);
  m_page = new Page(json["replies"].toObject(), this);
}
