#include "downloader.h"


Downloader::Downloader(QObject *parent) : QObject(parent) {}

void Downloader::downloadFile(const QUrl &url, const QString &destination) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(url);

    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::downloadProgress, this, &Downloader::downloadProgress);
    // Connect the signals to handle the download process
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            saveToFile(reply, destination);
            emit downloadFinished();
        } else {
            qDebug() << "Download failed:" << reply->errorString();
            emit downloadError();
        }

        // Clean up resources
        reply->deleteLater();
        manager->deleteLater();
    });
}


void Downloader::saveToFile(QNetworkReply *reply, const QString &destination) {
    QFile file(destination);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(reply->readAll());
        file.close();
        qDebug() << "Download complete. File saved to:" << destination;
    } else {
        qDebug() << "Failed to save file:" << file.errorString();
    }
}
