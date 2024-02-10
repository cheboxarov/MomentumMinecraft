#ifndef VERSIONS_H
#define VERSIONS_H
#include "QString"
#include "QVector"
struct Version {
    QString name;
    QString name_of_file;
    QString minecraft_version;
    QString url_to_download;
    QString description;
    QString photo;
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
