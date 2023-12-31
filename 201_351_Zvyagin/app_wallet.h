#ifndef APP_WALLET_H
#define APP_WALLET_H


#include "crypto.h"             // Подключаем криптографию
#include <QDialog>              // Для графики
#include <QDebug>               // Вывод
#include <QCryptographicHash>   // Хэш-функция
#include <QJsonArray>           //
#include <QJsonObject>          // Для работы с Json форматом
#include <QJsonDocument>        //
#include <QFile>
#include <QTimer>
#include <QPropertyAnimation>

//using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class app_wallet; }
QT_END_NAMESPACE

class app_wallet : public QDialog
{
    Q_OBJECT

public:
    app_wallet(QWidget *parent = nullptr);
    ~app_wallet();

    int index = 0;
    void create();          // Создание таблицы
    QList<Score> * scores = nullptr;
    QList<QString> colors = {"background-color: #ff0000;", "background-color: #66ff66;", "background-color: #ffff00;",
                             "background-color: #ff7700;", "background-color: #006699;"};
    void delay();

private:
    Ui::app_wallet *ui;
    QByteArray hex_hash;    // Ключ из хеш-функции пароля

private slots:
    void saveToFile();      // Сохранение состояния по выходу из приложения
    void on_prevBtn_clicked();
    void on_nextBtn_clicked();
};

#endif // app_wallet_H
