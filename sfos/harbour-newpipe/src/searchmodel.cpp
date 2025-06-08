#include "extractor.h"
#include "pageref.h"

#include "searchmodel.h"

SearchModel::SearchModel(QObject *parent)
  : QAbstractListModel(parent)
  , m_nextPage()
  , m_loading(false)
  , m_more(true)
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
  setLoading(false);
}

void SearchModel::append(QList<SearchItem const*> const& searchResults)
{
  beginInsertRows(QModelIndex(), m_searchResults.size(), m_searchResults.size() + searchResults.size() - 1);
  m_searchResults.append(searchResults);
  endInsertRows();
  setLoading(false);
}

void SearchModel::search(Extractor* extractor, QString const& searchTerm)
{
  if (m_nextPage) {
    m_nextPage = nullptr;
    delete m_nextPage;
  }
  setLoading(true);
  extractor->search(searchTerm);
}

void SearchModel::searchMore(Extractor* extractor, QString const& searchTerm)
{
  if (m_more) {
    if (!m_nextPage) {
      setLoading(true);
      extractor->search(searchTerm);
    }
    else {
      if (!m_nextPage->id().isEmpty()) {
        setLoading(true);
        extractor->searchMore(searchTerm, m_nextPage);
      }
    }
  }
}

PageRef* SearchModel::nextPage() const
{
  return m_nextPage;
}

void SearchModel::setNextPage(PageRef* nextPage)
{
  if (m_nextPage != nextPage) {
    if (m_nextPage && m_nextPage->parent() == this) {
      delete m_nextPage;
    }
    m_nextPage = nextPage;

    emit nextPageChanged();

    bool more = m_nextPage && !m_nextPage->id().isEmpty();
    setMore(more);
  }
}

bool SearchModel::loading() const
{
  return m_loading;
}

void SearchModel::setLoading(bool loading)
{
  if (m_loading != loading) {
    m_loading = loading;

    emit loadingChanged();
  }
}

bool SearchModel::more() const
{
  return m_more;
}

void SearchModel::setMore(bool more)
{
  if (m_more != more) {
    m_more = more;
    emit moreChanged();
  }
}
