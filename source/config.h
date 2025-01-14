#pragma once

#include <QJsonObject>

class Config {
public:
    static void loadConfiguration();
    static void storeConfiguration();
    static QJsonValue value(const QString &name);
    static void insert(const QString &key, const QJsonValue &value);
};
