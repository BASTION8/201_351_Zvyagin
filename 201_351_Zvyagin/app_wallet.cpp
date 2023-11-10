#include "app_wallet.h"
#include "ui_app_wallet.h"


app_wallet::app_wallet(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::app_wallet)
{
    ui->setupUi(this);

    // Ключ для шифрования (в задании не было сравнения с введенным)
    QByteArray hash = QCryptographicHash::hash("qwerty", QCryptographicHash::Sha256);
    hex_hash = hash.toBase64();

    create();
}


//заполнение таблицы
void app_wallet::create()
{
    // Обнуляем счетчики
    k = 0;
    summary = 0;

    // Расшифровываем файл и проверяем пуст ли он
    //scores = Crypto::decrypt_file(hex_hash);

    QJsonDocument json_doc;

    QFile file("scores.txt");
    file.open(QIODevice::ReadOnly);
    json_doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray array = json_doc.object().value("list").toArray();
    scores = new QList<Score>();
    for (auto item : array) {
        Score new_record;
        new_record.id = item.toObject().value("id").toString().toInt();
        new_record.summ = item.toObject().value("summ").toString().toInt();
        new_record.date = item.toObject().value("date").toString();
        scores->append(new_record);
        qDebug() << new_record.id << new_record.summ << new_record.date;
    }
    if (!scores->isEmpty()) {
        ui->idLabel->setText(QString::number(scores->toList()[0].id));
        ui->summLabel->setText(QString::number(scores->toList()[0].summ));
        ui->dateLabel->setText(scores->toList()[0].date);
    }
}

app_wallet::~app_wallet()
{
    delete ui;
}

// Перед выходом из программы сохраняем состояние игры
void app_wallet::saveToFile()
{
    QJsonDocument doc;
    QJsonArray array;

    for (Score item : *scores)
    {
        QJsonObject tmp;
        tmp.insert("id", item.id);
        tmp.insert("summ", item.summ);
        tmp.insert("date", item.date);
        array.append(tmp);
    }

    QJsonObject list;
    list.insert("list", array);
    doc.setObject(list);

    QFile file("scores.txt");
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson());
    file.close();

    // Шифруем файл
    if (Crypto::encrypt_file(doc.toJson(), hex_hash))
    {
        qDebug() << "Зашифрован!";
    }
    else
    {
        qDebug() << "Не зашифрован!";
        return;
    }
}


void app_wallet::on_prevBtn_clicked()
{

}


void app_wallet::on_nextBtn_clicked()
{

}
