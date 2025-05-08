#include "searchmodel.h"

SearchModel::SearchModel(QObject *parent) : QAbstractListModel(parent)
{
  roles[NameRole] = "name";
  roles[ThumbnailRole] = "thumbnail";
  roles[UrlRole] = "url";
}

QHash<int, QByteArray> SearchModel::roleNames() const
{
  return roles;
}

int SearchModel::rowCount(const QModelIndex & parent) const
{
  Q_UNUSED(parent)
  return searchResults.count();
}

QVariant SearchModel::data(const QModelIndex & index, int role) const
{
  QVariant result = QVariant();

  if ((index.row() >= 0) && (index.row() < searchResults.count())) {
    SearchItem const& searchResult = searchResults[index.row()];
    if (role == NameRole)
      result = searchResult.getName();
    else if (role == ThumbnailRole)
      result = searchResult.getThumbnail();
    else if (role == UrlRole)
      result = searchResult.getUrl();
  }

  return result;
}

void SearchModel::replaceAll(QList<SearchItem> const& searchResults)
{
  beginResetModel();
  this->searchResults.clear();
  this->searchResults.append(searchResults);
  endResetModel();
}
