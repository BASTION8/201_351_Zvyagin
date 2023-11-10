#include "crypto.h"

Crypto::Crypto()
{

}


QList<Score> * Crypto::decrypt_file(const QByteArray &key)
{
    QFile encrypted_file;
    encrypted_file.setFileName("D:\\scores.enc");
    encrypted_file.open(QIODevice::ReadWrite);
    if (!encrypted_file.isOpen())
    {
        return nullptr;
    }

    // Создание контекста шифрования (объявление)
    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new()))
    {
        return nullptr;
    }

    // Инициализация контекста
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key.data(), m_iv))
    {
        return nullptr;
    }

    QByteArray decoded_buf = QByteArray::fromBase64(encrypted_file.readAll());
    QBuffer buffer_stream(&decoded_buf);    // Из буффера берутся блоки по 256 байт
    buffer_stream.open(QBuffer::ReadOnly);

    int read_len = 0, decrypted_len = 0;
    QByteArray buffer_out;
    // Зацикливаем и расшифровываем по 256 байт из буфера
    while (true)
    {
        char encrypted_buf[256] = {0};
        unsigned char decrypted_buf[512] = {0};
        read_len = buffer_stream.read(encrypted_buf, 256);

        // Расшифруем
        if (1 != EVP_DecryptUpdate(ctx,
                                   decrypted_buf,                   // выходной параметр, буфер куда записывается расшифрованный текст
                                   &decrypted_len,                  // выходной параметр, длина расшифрованного текста
                                   (unsigned char *)encrypted_buf,  // входной параметр, буфер, зашифрованный текст
                                   read_len))                       // входной параметр, кол-во исходных байт
        {
            return nullptr;
        }

        if (read_len < 256)
        {
            // Если длина входного зашифрованного текста меньше 256, значит последний блок
            int tmplen;
            // Заполняет блок шифра и завершает шифрование
            if (!EVP_DecryptFinal_ex(ctx, decrypted_buf + decrypted_len, &tmplen))
            {
                EVP_CIPHER_CTX_free(ctx);
                return nullptr;
            }
            decrypted_len += tmplen;
            buffer_out += QByteArray((char *)decrypted_buf, decrypted_len);
            break;
        }
        else
        {
            buffer_out += QByteArray((char *)decrypted_buf, decrypted_len);
        }

    }
    // Конец работы с openssl
    EVP_CIPHER_CTX_free(ctx);

    encrypted_file.close();
    buffer_stream.close();

    QJsonDocument json_doc = QJsonDocument::fromJson(buffer_out);
    QJsonArray array = json_doc.object().value("list").toArray();
    QList<Score> * result = new QList<Score>();
    for (auto item : array) {
        Score new_record;
        new_record.id = item.toObject().value("id").toString().toInt();
        new_record.summ = item.toObject().value("summ").toString().toInt();
        new_record.date = item.toObject().value("date").toString().toUtf8();
        result->append(new_record);
    }

    return result;
}


bool Crypto::encrypt_file(const QByteArray plaintext, const QByteArray &key)
{
    // Создание контекста шифрования (объявление)
    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new()))
    {
        return 0;
    }

    // Инициализация контекста
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key.data(), m_iv))
    {
        return 0;
    }

    QByteArray not_yet_encoded = QByteArray(plaintext);
    QBuffer buffer_stream(&not_yet_encoded);
    buffer_stream.open(QBuffer::ReadOnly);

    QByteArray buffer_out;
    int read_len = 0, encrypted_len = 0;
    while (true) {
        unsigned char encrypted_buf[512] = {0};
        char decrypted_buf[256] = {0};
        read_len = buffer_stream.read(decrypted_buf, 256); // считать 256 байт

        // Шифрование
        if (1 != EVP_EncryptUpdate(ctx,
                                   encrypted_buf,                   // выходной параметр, буфер, куда записывается результат
                                   &encrypted_len,                  // выходной параметр, указатель, длина зашифрованного текста
                                   (unsigned char *) decrypted_buf, // входной параметр, буфер, открытый текст
                                   read_len))                       // входной параметр, кол-во байт, которые нужно зашифровать
        {
            return 0;
        }

        if (read_len < 256)
        {
            // Если длина входного зашифрованного текста меньше 256, значит последний блок
            int tmplen;
            // Заполняет блок шифра и завершает шифрование
            if (!EVP_EncryptFinal_ex(ctx, encrypted_buf + encrypted_len, &tmplen))
            {
                EVP_CIPHER_CTX_free(ctx);
                return false;
            }
            encrypted_len += tmplen;
            buffer_out += QByteArray((char *)encrypted_buf, encrypted_len);
            break;
        }
        else
        {
            buffer_out += QByteArray((char *)encrypted_buf, encrypted_len);
        }

    }
    EVP_CIPHER_CTX_free(ctx);
    buffer_stream.close();

    QFile encrypted_file;
    encrypted_file.setFileName("scores.enc");
    encrypted_file.open(QIODevice::WriteOnly);
    if (!encrypted_file.isOpen()) {
        return false;
    }
    encrypted_file.write(buffer_out.toBase64());
    encrypted_file.close();

    return true;
}
