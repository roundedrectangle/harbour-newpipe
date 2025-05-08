#ifndef SEARCHMODEL_H
#define SEARCHMODEL_H

#include <QAbstractListModel>

#include "searchitem.h"

class SearchModel : public QAbstractListModel
{
  Q_OBJECT
public:
  enum SearchRoles {
    NameRole = Qt::UserRole + 1,
    ThumbnailRole,
    UrlRole,
  };

  explicit SearchModel(QObject *parent = nullptr);

  QHash<int, QByteArray> roleNames() const;

  int rowCount(const QModelIndex & parent = QModelIndex()) const;

  QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

  void replaceAll(QList<SearchItem> const& searchResults);

private:
  QHash<int, QByteArray> roles;
  QList<SearchItem> searchResults;
};

#endif // SEARCHMODEL_H
