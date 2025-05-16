#include "commentitem.h"

CommentItem::CommentItem()
  : commentText()
  , uploaderName()
  , uploaderAvatar()
{

}

CommentItem::CommentItem(QString const& commentText, QString const& uploaderName, QString const& uploaderAvatar)
  : commentText(commentText)
  , uploaderName(uploaderName)
  , uploaderAvatar(uploaderAvatar)
{
}

QString CommentItem::getCommentText() const
{
  return commentText;
}

QString CommentItem::getUploaderName() const
{
  return uploaderName;
}

QString CommentItem::getUploaderAvatar() const
{
  return uploaderAvatar;
}

void CommentItem::setCommentText(QString const& commentText)
{
  this->commentText = commentText;
}

void CommentItem::setUploaderName(QString const& thumbnail)
{
  this->uploaderName = uploaderName;
}

void CommentItem::setUploaderAvatar(QString const& uploaderAvatar)
{
  this->uploaderAvatar = uploaderAvatar;
}
