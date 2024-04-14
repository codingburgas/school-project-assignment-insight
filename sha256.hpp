#pragma once
#include <QByteArray>
#include <QCryptographicHash>
#include <QString>
#include <QRandomGenerator>

class SHA256
{
public:
    SHA256();

    QString Hash(QString& password, QString& salt);
    QString GenerateSalt();
};
