#ifndef CRYPTO_H
#define CRYPTO_H

#include <QObject>
#include <QBuffer>      // Буффер, из него берем по порядку 256 байт
#include <QDebug>       // Для выводя в консоль
#include <QFile>        // Для работы с файлами
#include <openssl\evp.h>// OpenSSL для шифрования
#include <QJsonArray>   //
#include <QJsonObject>  // Для работы с Json форматом
#include <QJsonDocument>//

struct Score {
public:
    int id;
    int summ;
    QString date;
};

class Crypto
{
public:
    Crypto();

    static QList<Score> * decrypt_file(const QByteArray &key);
    // Шифрование файла
    static bool encrypt_file(const QByteArray plaintext,const QByteArray &key);

    constexpr static unsigned char m_iv[17] = "65e84be33532fb78";
};


#endif // CRYPTO_H
