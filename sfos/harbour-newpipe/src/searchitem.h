#ifndef SEARCHITEM_H
#define SEARCHITEM_H

#include <QString>

class SearchItem {
public:
  SearchItem();
  explicit SearchItem(QString const& name, QString const& thumbnail);

  QString getName() const;
  QString getThumbnail() const;

  void setName(QString const& name);
  void setThumbnail(QString const& thumbnail);

private:
  QString name;
  QString thumbnail;
};

#endif // SEARCHITEM_H
