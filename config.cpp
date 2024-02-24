#include "config.h"
#include "QFile"
#include "QCoreApplication"
#include "qregexp.h"
#include <fstream>

Config::Config() {}

void Config::saveConfig(QString nickname) {
    QFile file(QCoreApplication::applicationDirPath() + "/launcher.cfg");
    if (file.open(QIODevice::ReadWrite)) {
        QString text("nick=[" + nickname + "]");
        file.write(text.toStdString().c_str());
    }
    file.close();

    json config;

    config["name"] = nickname.toStdString();

    std::ofstream f("launch.json");
    f << std::setw(4) << config << std::endl;
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

    std::ifstream f("launch.json");
    if(f.is_open()) {
        qDebug() << "open";
    }
    try {
        json config = json::parse(f);
        return QString(config["nick"].get<std::string>().c_str());
    } catch (nlohmann::json_abi_v3_11_3::detail::type_error) {
        qDebug() << "Error to load config";
        return "Введите никнейм";
    }
}
