#ifndef DEBUG_WINDOW_H
#define DEBUG_WINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class debug_window; }
QT_END_NAMESPACE

class debug_window : public QDialog
{
    Q_OBJECT
public:
    explicit debug_window(QDialog *parent = nullptr);

private:
    Ui::debug_window *ui;
signals:

};

#endif // DEBUG_WINDOW_H
