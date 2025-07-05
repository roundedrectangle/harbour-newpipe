#ifndef CHANNELINFO_H
#define CHANNELINFO_H

#include <QObject>

class ListLinkHandler;

class ChannelInfo : public QObject
{
  Q_OBJECT
public:
  explicit ChannelInfo(QObject *parent = nullptr);
  explicit ChannelInfo(QString const& id, QString const& name, QString const& url, QString const& description, QList<ListLinkHandler*> const& tabs, QObject *parent = nullptr);
  explicit ChannelInfo(QJsonObject const& json, QObject *parent = nullptr);

public slots:
  QString id() const;
  QString name() const;
  QString url() const;
  QString description() const;
  QList<ListLinkHandler*> tabs();

  void setId(QString const& id);
  void setName(QString const& name);
  void setUrl(QString const& url);
  void setDescription(QString const& description);
  void setTabs(QList<ListLinkHandler*> const& tabs);

  void parseJson(QJsonObject const& json);

signals:

private:
  QString m_id;
  QString m_name;
  QString m_url;
  QString m_description;
  QList<ListLinkHandler*> m_tabs;
};

#endif // CHANNELINFO_H
