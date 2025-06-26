#include <QJsonObject>
#include <QDateTime>
#include <QLocale>
#include <QTranslator>
#include <QDebug>

#include "searchitemstream.h"

SearchItemStream::SearchItemStream(QObject *parent)
  : SearchItem(parent)
  , m_uploaderName()
  , m_uploadDate(0)
{
  m_infoType = Stream;
}

SearchItemStream::SearchItemStream(QJsonObject const& json, QObject *parent)
  : SearchItem(parent)
{
  m_infoType = Stream;
  parseJson(json);
}

QString SearchItemStream::uploaderName() const
{
  return m_uploaderName;
}

quint64 SearchItemStream::uploadDate() const
{
  return m_uploadDate;
}

QString SearchItemStream::textualUploadDate() const
{
  return m_textualUploadDate;
}

void SearchItemStream::setUploaderName(QString const& uploaderName)
{
  m_uploaderName = uploaderName;
}

void SearchItemStream::setUploadDate(quint64 uploadDate)
{
  m_uploadDate = uploadDate;
}

void SearchItemStream::setTextualUploadDate(QString const& textualUploadDate)
{
  m_textualUploadDate = textualUploadDate;
}

void SearchItemStream::parseJson(QJsonObject const& json)
{
  SearchItem::parseJson(json);

  m_uploaderName = json["uploaderName"].toString();
  m_uploadDate = json["uploadDate"].toObject()["offsetDateTime"].toInt(0);
  m_textualUploadDate = json["textualUploadDate"].toString();
}

QString SearchItemStream::getInfoRow() const
{
  //% "%0 • %1"
  QString const& format = qtTrId("newpipe-searchitem-stream_inforow");
  return QString(format).arg(m_textualUploadDate).arg(m_uploaderName);
}
