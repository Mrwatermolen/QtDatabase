#include "customNetworkDownloader.h"

#include <QNetworkRequest>

customNetworkDownloader::customNetworkDownloader(QObject *parent)
    : QObject(parent),
      m_pNetWorkManager(new QNetworkAccessManager(this)) {}

void customNetworkDownloader::get(const QString &url) {
  // Note: After the request has finished, it is the responsibility of the user
  // to delete the QNetworkReply object at an appropriate time. Do not directly
  // delete it inside the slot connected to finished(). You can use the
  // deleteLater() function.
  auto request{QNetworkRequest()};
  request.setUrl(url);

  m_data.clear();

  m_pReply = m_pNetWorkManager->get(request);

  connect(m_pReply, &QNetworkReply::readyRead, this,
          &customNetworkDownloader::onNetworkRelyReadyRead);
  connect(m_pReply, &QNetworkReply::errorOccurred, this,
          &customNetworkDownloader::onNetworkRelyError);
  connect(m_pReply, &QNetworkReply::downloadProgress, this,
          &customNetworkDownloader::onNetworkRelyDownload);
  connect(m_pReply, &QNetworkReply::finished, this,
          &customNetworkDownloader::onNetworkRelyFinished);
}

QByteArray customNetworkDownloader::getData() { return m_data; }

void customNetworkDownloader::onNetworkRelyReadyRead() {
  qDebug() << "customNetworkDownloader::onNetworkRelyReadyRead()";
  m_data.append(m_pReply->readAll());
}

void customNetworkDownloader::onNetworkRelyError(
    QNetworkReply::NetworkError err) {
  qDebug() << "customNetworkDownloader::onNetworkRelyError() Error:" << err;
}

void customNetworkDownloader::onNetworkRelyDownload(qint64 bytesReceived,
                                                  qint64 bytesTotal) {
  qDebug() << "customNetworkDownloader::onNetworkRelyDownload() bytesReceived"
           << bytesReceived << "bytesTotal" << bytesTotal;
}

void customNetworkDownloader::onNetworkRelyFinished() {
  qDebug() << "customNetworkDownloader::onNetworkRelyFinished()";
  delete m_pReply;
  m_pReply = nullptr;
}