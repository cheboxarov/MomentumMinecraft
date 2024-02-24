#ifndef VERSIONS_H
#define VERSIONS_H
#include "QString"
#include "QVector"
#include "downloader.h"

struct Version {
    Version() {};
    Version(bool isNull) : isNull(isNull) {}
    QString name;
    QString name_of_file;
    QString minecraft_version;
    QString url_to_download;
    QString description;
    QString photo;
    std::string startcmd;
    int size;
    int id;
    bool isNull = false;
};

class Versions : public QObject
{
    Q_OBJECT
public:
    Versions(QObject *parent = nullptr);
    virtual ~Versions() {}
    void startDownloadVersions();
    QVector<Version> getLoadedVersions();
private:
    Downloader *downloader;
    QVector<Version> loaded_versions;
public slots:
    void loadVersions();
    void loadError();
signals:
    void versionsLoaded(bool withError);
};

#endif // VERSIONS_H
