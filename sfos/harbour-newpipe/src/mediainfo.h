#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include <QObject>

class MediaInfo : public QObject
{
  Q_OBJECT
public:
  explicit MediaInfo(QObject *parent = nullptr);
  explicit MediaInfo(QString const& url, QString const& name, QString const& uploaderName, QString const& category, int viewCount, int likeCount, QString const& content, QObject *parent = nullptr);

public slots:
  QString getUrl() const;
  QString getName() const;
  QString getUploaderName() const;
  QString getCategory() const;
  int getViewCount() const;
  int getLikeCount() const;
  QString getContent() const;

  void setUrl(QString const& url);
  void setName(QString const& name);
  void setUploaderName(QString const& uploaderName);
  void setCategory(QString const& category);
  void setViewCount(int viewCount);
  void setLikeCount(int likeCount);
  void setContent(QString const& content);

signals:

private:
  QString url;
  QString name;
  QString uploaderName;
  QString category;
  int viewCount;
  int likeCount;
  QString content;
};

#endif // MEDIAINFO_H
