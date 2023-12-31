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

void app_wallet::create()
{
//    QJsonDocument json_doc;

//    QFile file("scores.txt");
//    file.open(QIODevice::ReadOnly);
//    json_doc = QJsonDocument::fromJson(file.readAll());
//    file.close();

//    QJsonArray array = json_doc.object().value("list").toArray();
//    scores = new QList<Score>();
//    for (auto item : array) {
//        Score new_record;
//        new_record.id = item.toObject().value("id").toString().toInt();
//        new_record.summ = item.toObject().value("summ").toString().toInt();
//        new_record.date = item.toObject().value("date").toString();
//        scores->append(new_record);
//        qDebug() << new_record.id << new_record.summ << new_record.date;
//    }

    scores = Crypto::decrypt_file(hex_hash);

    if (!scores->isEmpty()) {
        ui->idLabel->setText(QString::number(scores->toList()[0].id));
        ui->summLabel->setText(QString::number(scores->toList()[0].summ));
        ui->dateLabel->setText(scores->toList()[0].date);
        ui->label->setStyleSheet(colors[index % 5]);
    }
    index = 0;
    saveToFile();
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
        tmp.insert("id", QString::number(item.id));
        tmp.insert("summ", QString::number(item.summ));
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
    if (index > 0)
    {
        QRect geo = ui->swipeWidget->geometry();
        QPropertyAnimation *animation = new QPropertyAnimation(ui->swipeWidget, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(geo);
        animation->setEndValue(QRect(1000, geo.y(), geo.width(), geo.height()));
        animation->start();

        delay();

        ui->swipeWidget->setGeometry(QRect(-1000, geo.y(), geo.width(), geo.height()));

        animation->setDuration(1000);
        animation->setStartValue(QRect(-1000, geo.y(), geo.width(), geo.height()));
        animation->setEndValue(geo);
        animation->start();

        index -= 1;
        ui->idLabel->setText(QString::number(scores->toList()[index].id));
        ui->summLabel->setText(QString::number(scores->toList()[index].summ));
        ui->dateLabel->setText(scores->toList()[index].date);
        ui->label->setStyleSheet(colors[index % 5]);

        delay();
    }
    else
    {
        return;
    }
}


void app_wallet::on_nextBtn_clicked()
{
    if (index < scores->length() - 1)
    {
        QRect geo = ui->swipeWidget->geometry();
        QPropertyAnimation *animation = new QPropertyAnimation(ui->swipeWidget, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(geo);
        animation->setEndValue(QRect(-1000, geo.y(), geo.width(), geo.height()));
        animation->start();

        delay();

        ui->swipeWidget->setGeometry(QRect(1000, geo.y(), geo.width(), geo.height()));

        animation->setDuration(1000);
        animation->setStartValue(QRect(1000, geo.y(), geo.width(), geo.height()));
        animation->setEndValue(geo);
        animation->start();

        index += 1;
        ui->idLabel->setText(QString::number(scores->toList()[index].id));
        ui->summLabel->setText(QString::number(scores->toList()[index].summ));
        ui->dateLabel->setText(scores->toList()[index].date);
        ui->label->setStyleSheet(colors[index % 5]);

        delay();
    }
    else
    {
        return;
    }
}

void app_wallet::delay() {
    QTime dieTime = QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
