#include "sha256.hpp"
#include <QRandomGenerator> // Include the necessary headers

SHA256::SHA256() {
}

QString SHA256::Hash(QString &secret, QString &salt) {
    QByteArray hashWithSalt = (secret + salt).toUtf8();
    QByteArray hashed = QCryptographicHash::hash(hashWithSalt, QCryptographicHash::Sha256);
    // Convert the hashed QByteArray to QString before returning
    return QString::fromUtf8(hashed.toHex());
}

QString SHA256::GenerateSalt() {
    // Generate a random salt
    QByteArray salt;
    for (int i = 0; i < 16; ++i) {
        salt.append(QRandomGenerator::global()->generate());
    }
    // Convert the salt QByteArray to QString before returning
    return QString::fromUtf8(salt.toHex());
}
