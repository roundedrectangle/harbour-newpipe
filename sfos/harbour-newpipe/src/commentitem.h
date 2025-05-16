#ifndef COMMENTITEM_H
#define COMMENTITEM_H

#include <QString>

class CommentItem
{
public:
  CommentItem();
  explicit CommentItem(QString const& commentText, QString const& uploaderName, QString const& uploaderAvatar);

  QString getCommentText() const;
  QString getUploaderName() const;
  QString getUploaderAvatar() const;

  void setCommentText(QString const& commentText);
  void setUploaderName(QString const& thumbnail);
  void setUploaderAvatar(QString const& uploaderAvatar);

private:
  QString commentText;
  QString uploaderName;
  QString uploaderAvatar;
};

#endif // COMMENTITEM_H
