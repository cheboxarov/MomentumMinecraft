#ifndef FTPDOWNLOADER_H
#define FTPDOWNLOADER_H

#include <QObject>
#include <QUrl>

class FtpDownloader : public QObject
{
    Q_OBJECT

public:
    FtpDownloader(QObject *parent = nullptr);

public slots:
    void downloadDirectory(const QUrl &ftpUrl, const QString &outputPath, const QString &username, const QString &password);

private:
         // Дополнительные члены класса, если необходимо
};

#endif // FTPDOWNLOADER_H
