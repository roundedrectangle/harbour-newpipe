#ifndef COMMENTMODEL_H
#define COMMENTMODEL_H

#include <QAbstractListModel>

#include "commentitem.h"

class CommentModel : public QAbstractListModel
{
  Q_OBJECT
public:
  enum CommentRoles {
    CommentTextRole = Qt::UserRole + 1,
    UploaderNameRole,
    UploaderAvatarRole,
    ReplyCountRole,
    PageRole,
  };

  explicit CommentModel(QObject *parent = nullptr);

  QHash<int, QByteArray> roleNames() const;

  int rowCount(const QModelIndex & parent = QModelIndex()) const;

  QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

  void replaceAll(QList<CommentItem const*> const& commentResults);

  void setPage(PageRef* page);

private:
  QHash<int, QByteArray> m_roles;
  QList<CommentItem const*> m_commentResults;
  PageRef* m_page;
};

#endif // COMMENTMODEL_H
