#ifndef EDITWORKSWINDOW_H
#define EDITWORKSWINDOW_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>

#include "customproxymodel.h"
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
    void AuthoWindow();
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
    CreateWork *newWork;
    EditWork *editWork;
//    AbstractWork *editWork;
    CustomBox msgbx;
    QSortFilterProxyModel *proxyModel;
};

#endif // EDITWORKSWINDOW_H
