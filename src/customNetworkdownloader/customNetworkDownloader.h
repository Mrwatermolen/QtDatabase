#ifndef _customNetworkDownloader_H_
#define _customNetworkDownloader_H_

#include <QNetworkAccessManager>
#include <QNetworkReply>

class customNetworkDownloader : public QObject {
  Q_OBJECT // 允许signal
      public : customNetworkDownloader(QObject *parent = nullptr);
  ~customNetworkDownloader() = default;

public:
  void get(const QString &fileName);
  QByteArray getData();

private:
  QNetworkAccessManager *m_pNetWorkManager{nullptr};
  QNetworkReply *m_pReply{nullptr};
  QByteArray m_data;

private slots:
  void onNetworkRelyReadyRead();
  void onNetworkRelyError(QNetworkReply::NetworkError err);
  void onNetworkRelyDownload(qint64 bytesReceived, qint64 bytesTotal);
  void onNetworkRelyFinished();
};

#endif // _customNetworkDownloader_H_