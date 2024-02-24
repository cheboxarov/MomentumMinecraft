#include "versions.h"
#include "QCoreApplication"
#include "json.hpp"
#include "fstream"
#include "QString"
#include "windows.h"


using json = nlohmann::json;

Versions::Versions(QObject *parent) : QObject(parent), downloader(new Downloader(this)) {}

QVector<Version> Versions::getLoadedVersions() {
    return loaded_versions;
}

void Versions::startDownloadVersions() {
    downloader->downloadFile(QUrl("https://minecraft_chebox.hb.ru-msk.vkcs.cloud/versions.json"), QCoreApplication::applicationDirPath() + "/versions.json");
    QObject::connect(downloader, &Downloader::downloadFinished, this, &Versions::loadVersions);
    QObject::connect(downloader, &Downloader::downloadError, this, &Versions::loadError);
}

void Versions::loadVersions() {
    std::ifstream f("versions.json");
    json versions_json;
    try {
        versions_json = json::parse(f);
    } catch (nlohmann::json_abi_v3_11_3::detail::parse_error) {
        qDebug() << "Error with load versions";
        return;
    }
    for(int i = 0; i < versions_json.size(); ++i) {
        Version ver;
        ver.name = QString(versions_json[i]["name"].get<std::string>().c_str());
        ver.description = QString(versions_json[i]["description"].get<std::string>().c_str());
        ver.name_of_file = QString(versions_json[i]["name_of_file"].get<std::string>().c_str());
        ver.startcmd = versions_json[i]["startcmd"].get<std::string>();
        ver.url_to_download = QString(versions_json[i]["url_to_download"].get<std::string>().c_str());
        ver.minecraft_version = QString(versions_json[i]["minecraft_version"].get<std::string>().c_str());
        ver.photo = QString(versions_json[i]["photo"].get<std::string>().c_str());
        ver.size = versions_json[i]["size"].get<int>();
        ver.id = versions_json[i]["id"].get<int>();
        loaded_versions.append(ver);
    }
    emit versionsLoaded(false);
}

void Versions::loadError() {
    emit versionsLoaded(true);
}
