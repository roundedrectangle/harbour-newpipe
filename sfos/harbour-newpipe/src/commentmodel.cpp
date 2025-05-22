#include "pageref.h"

#include "commentmodel.h"

CommentModel::CommentModel(QObject *parent) : QAbstractListModel(parent)
{
  m_roles[CommentTextRole] = "commentText";
  m_roles[UploaderNameRole] = "uploaderName";
  m_roles[UploaderAvatarRole] = "uploaderAvatar";
  m_roles[ReplyCountRole] = "replyCount";
  m_roles[PageRole] = "page";
}

QHash<int, QByteArray> CommentModel::roleNames() const
{
  return m_roles;
}

int CommentModel::rowCount(const QModelIndex & parent) const
{
  Q_UNUSED( parent)
  return m_commentResults.count();
}

QVariant CommentModel::data(const QModelIndex & index, int role) const
{
  QVariant result = QVariant();

  if ((index.row() >= 0) && (index.row() < m_commentResults.count())) {
    CommentItem const* commentResult = m_commentResults[index.row()];
    if (role == CommentTextRole)
      result = commentResult->getCommentText();
    else if (role == UploaderNameRole)
      result = commentResult->getUploaderName();
    else if (role == UploaderAvatarRole)
      result = commentResult->getUploaderAvatar();
    else if (role == ReplyCountRole)
      result = commentResult->getReplyCount();
    else if (role == PageRole)
      result = QVariant::fromValue(commentResult->getPage());
  }

  return result;
}

void CommentModel::replaceAll(QList<CommentItem const*> const& commentResults)
{
  beginResetModel();
  for (CommentItem const* commentItem : m_commentResults) {
    delete commentItem;
  }
  m_commentResults.clear();
  m_commentResults.append(commentResults);
  endResetModel();
}

