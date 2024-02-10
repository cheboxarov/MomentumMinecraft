#include "config.h"
#include "QFile"
#include "QCoreApplication"
#include "qregexp.h"
Config::Config() {}

void Config::saveConfig(QString nickname) {
    QFile file(QCoreApplication::applicationDirPath() + "/launcher.cfg");
    if (file.open(QIODevice::ReadWrite)) {
        QString text("nick=[" + nickname + "]");
        file.write(text.toStdString().c_str());
    }
    file.close();
}

QString Config::getNickname() {
    QString nick;
    QFile file(QCoreApplication::applicationDirPath() + "/launcher.cfg");
    if (file.open(QIODevice::ReadWrite)) {
        QRegExp rx("nick=\\[(\\S+)\\]");
        char nickname[256];
        file.read(nickname, file.size());
        int result = rx.indexIn(QString(nickname));
        if (result == -1)
            return "Твой никнейм";
        else {
            for (int i = result + 6; i <= rx.matchedLength(); ++i) {
                if (nickname[i] == ']')
                    break;
                nick += nickname[i];
            }

        }
    }
    file.close();
    qDebug() << nick;
    return nick;
}
