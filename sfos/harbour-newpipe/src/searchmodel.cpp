#include "searchmodel.h"

SearchModel::SearchModel(QObject *parent) : QAbstractListModel(parent)
{
  m_roles[NameRole] = "name";
  m_roles[ThumbnailRole] = "thumbnail";
  m_roles[UrlRole] = "url";
}

QHash<int, QByteArray> SearchModel::roleNames() const
{
  return m_roles;
}

int SearchModel::rowCount(const QModelIndex & parent) const
{
  Q_UNUSED(parent)
  return m_searchResults.count();
}

QVariant SearchModel::data(const QModelIndex & index, int role) const
{
  QVariant result = QVariant();

  if ((index.row() >= 0) && (index.row() < m_searchResults.count())) {
    SearchItem const* searchResult = m_searchResults[index.row()];
    if (role == NameRole)
      result = searchResult->getName();
    else if (role == ThumbnailRole)
      result = searchResult->getThumbnail();
    else if (role == UrlRole)
      result = searchResult->getUrl();
  }

  return result;
}

void SearchModel::replaceAll(QList<SearchItem const*> const& searchResults)
{
  beginResetModel();
  for (SearchItem const* searchItem : m_searchResults) {
    delete searchItem;
  }
  m_searchResults.clear();
  m_searchResults.append(searchResults);
  endResetModel();
}
