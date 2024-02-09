#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>

class Downloader : public QObject
{
    Q_OBJECT

public:
    Downloader(QObject *parent = nullptr);

public slots:
    void downloadFile(const QUrl &url, const QString &destination);
    void saveToFile(QNetworkReply *reply, const QString &destination);
signals:
    void downloadProgress(qint64 bytesSent, qint64 bytesTotal);
    void downloadFinished();
};
#endif // DOWNLOADER_H
