#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include <QObject>

class MediaInfo : public QObject
{
  Q_OBJECT
public:
  explicit MediaInfo(QObject *parent = nullptr);
  explicit MediaInfo(QString const& url, QString const& name, QString const& uploaderName, QString const& category, int viewCount, int likeCount, QString const& content, QObject *parent = nullptr);
  explicit MediaInfo(QJsonObject const& json, QObject *parent = nullptr);

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

  void parseJson(QJsonObject const& json);

private:
  QString m_url;
  QString m_name;
  QString m_uploaderName;
  QString m_category;
  int m_viewCount;
  int m_likeCount;
  QString m_content;
};

#endif // MEDIAINFO_H
