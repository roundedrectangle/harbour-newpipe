#include "commentmodel.h"

CommentModel::CommentModel(QObject *parent) : QAbstractListModel(parent)
{
  roles[CommentTextRole] = "commentText";
  roles[UploaderNameRole] = "uploaderName";
  roles[UploaderAvatarRole] = "uploaderAvatar";
}

QHash<int, QByteArray> CommentModel::roleNames() const
{
  return roles;
}

int CommentModel::rowCount(const QModelIndex & parent) const
{
  Q_UNUSED(parent)
  return commentResults.count();
}

QVariant CommentModel::data(const QModelIndex & index, int role) const
{
  QVariant result = QVariant();

  if ((index.row() >= 0) && (index.row() < commentResults.count())) {
    CommentItem const& commentResult = commentResults[index.row()];
    if (role == CommentTextRole)
      result = commentResult.getCommentText();
    else if (role == UploaderNameRole)
      result = commentResult.getUploaderName();
    else if (role == UploaderAvatarRole)
      result = commentResult.getUploaderAvatar();
  }

  return result;
}

void CommentModel::replaceAll(QList<CommentItem> const& commentResults)
{
  beginResetModel();
  this->commentResults.clear();
  this->commentResults.append(commentResults);
  endResetModel();
}
