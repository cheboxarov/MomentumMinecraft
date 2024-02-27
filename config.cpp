#include "config.h"
#include "QFile"
#include "QCoreApplication"
#include "qregexp.h"
#include <fstream>

Config::Config() {}

void Config::saveConfig(QString nickname) {
    json config;
    config["name"] = nickname.toStdString();
    config["version"] = 1;
    std::ofstream f("launch.json");
    f << std::setw(4) << config << std::endl;
    f.close();
}

QString Config::getNickname() {
    // QString nick;
    // QFile file(QCoreApplication::applicationDirPath() + "/launcher.cfg");
    // if (file.open(QIODevice::ReadWrite)) {
    //     QRegExp rx("nick=\\[(\\S+)\\]");
    //     char nickname[256];
    //     file.read(nickname, file.size());
    //     int result = rx.indexIn(QString(nickname));
    //     if (result == -1)
    //         return "Твой никнейм";
    //     else {
    //         for (int i = result + 6; i <= rx.matchedLength(); ++i) {
    //             if (nickname[i] == ']')
    //                 break;
    //             nick += nickname[i];
    //         }

    //     }
    //     file.close();
    // }
    QString nick;
    json config;
    std::ifstream f("launch.json");
    // if(f.is_open()) {
    //     qDebug() << "open";
    //     std::string line;
    //     while(std::getline(f, line)) {
    //         qDebug() << line;
    //     }
    // }
    try {
        f >> config;
        auto el = config["name"].get<std::string>().c_str();
        qDebug() << el;
        return el;
    } catch (nlohmann::json_abi_v3_11_3::detail::parse_error) {
        qDebug() << "Error to load config 1";
        return "Введите никнейм";
    } catch (nlohmann::json_abi_v3_11_3::detail::type_error) {
        qDebug() << "Error to load config 2";
        return "Введите никнейм";
    }
}
