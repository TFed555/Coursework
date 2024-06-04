#ifndef EDITWORKSWINDOW_H
#define EDITWORKSWINDOW_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>

#include "editsproxymodel.h"
#include "worksmodel.h"
#include "custombox.h"
#include "creatework.h"
#include "editwork.h"

namespace Ui {
class EditWorksWindow;
}

class EditWorksWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EditWorksWindow(QWidget *parent = nullptr);
    ~EditWorksWindow();

signals:
    void OrganiserWindow();
    void updateUsers();
private slots:
    void on_backButton_clicked();
    void on_addButton_clicked();
    void on_delButton_clicked();
    void showWork(const QModelIndex &index);
    void on_finishButton_clicked();

private:

    void createUI();

private:
    Ui::EditWorksWindow *ui;
    WorksModel *mymodel;
    std::shared_ptr <CreateWork> newWork;
    std::shared_ptr <EditWork> editWork;
    CustomBox msgbx;
    std::shared_ptr <DataBase> db;
    std::shared_ptr <EditsSortFilterProxyModel> proxyModel;
};

#endif // EDITWORKSWINDOW_H
