#include "ftpdownloader.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

FtpDownloader::FtpDownloader(QObject *parent) : QObject(parent) {}

void FtpDownloader::downloadDirectory(const QUrl &ftpUrl, const QString &outputPath, const QString &username, const QString &password)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // Включаем имя пользователя и пароль в URL
    QUrl urlWithCredentials = ftpUrl;
    urlWithCredentials.setUserName(username);
    urlWithCredentials.setPassword(password);

    QNetworkRequest request(urlWithCredentials);

    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error()) {
            qDebug() << "Download failed:" << reply->errorString();
        } else {
            QByteArray data = reply->readAll();
            QString fileName = outputPath + "/" + QFileInfo(ftpUrl.path()).fileName();

            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(data);
                file.close();
                qDebug() << "Download complete";

                // Process the downloaded directory or files as needed
                // ...

            } else {
                qDebug() << "Failed to open file for writing:" << fileName;
            }
        }

        reply->deleteLater();
        manager->deleteLater();
    });
}
