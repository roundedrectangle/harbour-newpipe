#ifndef COMMENTITEM_H
#define COMMENTITEM_H

#include <QObject>
#include <QString>

class Page;

class CommentItem : QObject
{
  Q_OBJECT
public:
  CommentItem(QObject *parent = nullptr);
  explicit CommentItem(QString const& commentText, QString const& uploaderName, QString const& uploaderAvatar, qint64 replyCount, Page *m_page, QObject *parent = nullptr);
  explicit CommentItem(QJsonObject const& json, QObject *parent = nullptr);

  QString getCommentText() const;
  QString getUploaderName() const;
  QString getUploaderAvatar() const;
  qint64 getReplyCount() const;
  Page* getPage() const;

  void setCommentText(QString const& commentText);
  void setUploaderName(QString const& thumbnail);
  void setUploaderAvatar(QString const& uploaderAvatar);
  void setReplyCount(qint64 replyCount);
  void setPage(Page* page);

  void parseJson(QJsonObject const& json);

private:
  QString m_commentText;
  QString m_uploaderName;
  QString m_uploaderAvatar;
  qint64 m_replyCount;
  Page* m_page;
};

#endif // COMMENTITEM_H
