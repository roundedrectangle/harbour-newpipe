#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include <QObject>
#include <QMetaMethod>

class MediaInfo : public QObject
{
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QString uploaderName READ uploaderName WRITE setUploaderName NOTIFY uploaderNameChanged)
  Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY categoryChanged)
  Q_PROPERTY(qint64 viewCount READ viewCount WRITE setViewCount NOTIFY viewCountChanged)
  Q_PROPERTY(qint64 likeCount READ likeCount WRITE setLikeCount NOTIFY likeCountChanged)
  Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)

  Q_OBJECT
public:
  explicit MediaInfo(QObject *parent = nullptr);
  explicit MediaInfo(QString const& url, QString const& name, QString const& uploaderName, QString const& category, qint64 viewCount, qint64 likeCount, QString const& content, QObject *parent = nullptr);
  explicit MediaInfo(QJsonObject const& json, QObject *parent = nullptr);

public slots:
  QString name() const;
  QString uploaderName() const;
  QString category() const;
  qint64 viewCount() const;
  qint64 likeCount() const;
  QString content() const;

  void setName(QString const& name);
  void setUploaderName(QString const& uploaderName);
  void setCategory(QString const& category);
  void setViewCount(qint64 viewCount);
  void setLikeCount(qint64 likeCount);
  void setContent(QString const& content);

  void parseJson(QJsonObject const& json);

private:
  typedef void (MediaInfo::*MediaInfoSignal)();

  QList<MediaInfoSignal> parseJsonChanges(QJsonObject const& json);

signals:
  void nameChanged();
  void uploaderNameChanged();
  void categoryChanged();
  void viewCountChanged();
  void likeCountChanged();
  void contentChanged();

private:
  QString m_name;
  QString m_uploaderName;
  QString m_category;
  qint64 m_viewCount;
  qint64 m_likeCount;
  QString m_content;
};

#endif // MEDIAINFO_H
