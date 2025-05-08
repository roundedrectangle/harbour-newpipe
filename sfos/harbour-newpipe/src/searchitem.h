#ifndef SEARCHITEM_H
#define SEARCHITEM_H

#include <QString>

class SearchItem {
public:
  SearchItem();
  explicit SearchItem(QString const& name, QString const& thumbnail, QString const& url);

  QString getName() const;
  QString getThumbnail() const;
  QString getUrl() const;

  void setName(QString const& name);
  void setThumbnail(QString const& thumbnail);
  void setUrl(QString const& thumbnail);

private:
  QString name;
  QString thumbnail;
  QString url;
};

#endif // SEARCHITEM_H
