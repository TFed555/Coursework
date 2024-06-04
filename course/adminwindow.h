#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QDialog>
#include "userswindow.h"

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

public:
    void update_model();

signals:
    void AuthoWindow();

private slots:
    void on_nextButton_clicked();

    void on_backButton_clicked();

private:
    Ui::AdminWindow *ui;
    std::shared_ptr <UsersWindow> adminUi;
};

#endif // ADMINWINDOW_H
