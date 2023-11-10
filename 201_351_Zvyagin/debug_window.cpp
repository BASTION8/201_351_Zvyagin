#include "debug_window.h"
#include "ui_debug_window.h"

debug_window::debug_window(QDialog *parent)
    : QDialog{parent}
    , ui(new Ui::debug_window)
{
    ui->setupUi(this);
}
