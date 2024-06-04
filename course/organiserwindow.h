#ifndef ORGANISERWINDOW_H
#define ORGANISERWINDOW_H

#include <QDialog>
#include "editworkswindow.h"

namespace Ui {
class OrganiserWindow;
}

class OrganiserWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OrganiserWindow(QWidget *parent = nullptr);
    ~OrganiserWindow();

signals:
    void updateUsersAutho();
    void AuthoWindow();
private slots:
    void on_nextButton_clicked();

    void on_backButton_clicked();

    void update_users();

private:
    Ui::OrganiserWindow *ui;
    std::shared_ptr <EditWorksWindow> editUi;
};

#endif // ORGANISERWINDOW_H
