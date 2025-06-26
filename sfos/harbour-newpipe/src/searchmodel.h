#ifndef SEARCHMODEL_H
#define SEARCHMODEL_H

#include <QAbstractListModel>

#include "searchitem.h"

class PageRef;
class Extractor;

class SearchModel : public QAbstractListModel
{
  Q_PROPERTY(PageRef* nextPage READ nextPage WRITE setNextPage NOTIFY nextPageChanged)
  Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)
  Q_PROPERTY(bool more READ more WRITE setMore NOTIFY moreChanged)

  Q_OBJECT
public:
  enum SearchRoles {
    NameRole = Qt::UserRole + 1,
    ThumbnailRole,
    UrlRole,
    InfoTypeRole,
    InfoRowRole,
  };

  explicit SearchModel(QObject *parent = nullptr);

  QHash<int, QByteArray> roleNames() const;

  int rowCount(const QModelIndex & parent = QModelIndex()) const;

  QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

  void replaceAll(QList<SearchItem const*> const& searchResults);

  void append(QList<SearchItem const*> const& searchResults);

public slots:
  void search(Extractor* extractor, QString const& searchTerm);
  void searchMore(Extractor* extractor, QString const& searchTerm);

  PageRef* nextPage() const;
  void setNextPage(PageRef* nextPage);

  bool loading() const;
  void setLoading(bool loading);

  bool more() const;
  void setMore(bool more);

signals:
  void nextPageChanged();
  void loadingChanged();
  void moreChanged();

private:
  QHash<int, QByteArray> m_roles;
  QList<SearchItem const*> m_searchResults;
  PageRef* m_nextPage;
  bool m_loading;
  bool m_more;
};

#endif // SEARCHMODEL_H
