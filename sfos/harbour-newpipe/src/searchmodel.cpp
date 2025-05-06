#include "searchmodel.h"

SearchModel::SearchModel(QObject *parent) : QAbstractListModel(parent)
{
  roles[NameRole] = "name";
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
    QString const& searchResult = searchResults[index.row()];
    if (role == NameRole)
        result = searchResult;
  }

  return result;
}

void SearchModel::replaceAll(QStringList searchResults)
{
  beginResetModel();
  this->searchResults.clear();
  this->searchResults.append(searchResults);
  endResetModel();
}
