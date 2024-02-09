#ifndef VERSIONS_H
#define VERSIONS_H
#include "QString"
#include "QVector"
struct Version {
    Version(QString nameVal, QString description, QString minecraft_versionVal,
            QString url_to_downloadVal, int sizeVal, int idVal, QString name_of_file, char* startcmd) :
        name(nameVal)
        ,minecraft_version(minecraft_versionVal)
        ,url_to_download(url_to_downloadVal)
        ,size(sizeVal)
        ,id(idVal)
        ,description(description)
        ,name_of_file(name_of_file)
        ,startcmd(startcmd)   {}
    QString name;
    QString name_of_file;
    QString minecraft_version;
    QString url_to_download;
    QString description;
    char* startcmd;
    int size;
    int id;
};

class Versions
{
public:
    Versions();
    void loadVersions();
    QVector<Version> loaded_versions;
};

#endif // VERSIONS_H
