#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    wallet = new app_wallet;
    ui->error->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Авторизация
void MainWindow::on_Ok_clicked()
{
    // Если пароль равен
    if (ui->code_text->text() == "qwerty")
    {
        this->close();
        wallet->show();
    }
    else
    {
        ui->error->setVisible(true);
    }
}
