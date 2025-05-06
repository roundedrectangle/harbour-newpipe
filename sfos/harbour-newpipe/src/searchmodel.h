#ifndef SEARCHMODEL_H
#define SEARCHMODEL_H

#include <QAbstractListModel>

class SearchModel : public QAbstractListModel
{
  Q_OBJECT
public:
  enum SearchRoles {
      NameRole = Qt::UserRole + 1,
  };

  explicit SearchModel(QObject *parent = nullptr);

  QHash<int, QByteArray> roleNames() const;

  int rowCount(const QModelIndex & parent = QModelIndex()) const;

  QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

  void replaceAll(QStringList searchResults);

private:
  QHash<int, QByteArray> roles;
  QStringList searchResults;
};

#endif // SEARCHMODEL_H
