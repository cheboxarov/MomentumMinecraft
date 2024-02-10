#ifndef CONFIG_H
#define CONFIG_H

#include "QString"

class Config
{
public:
    Config();
    static void saveConfig(QString nickname);
    static QString getNickname();
};

#endif // CONFIG_H
