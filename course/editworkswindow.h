#ifndef EDITWORKSWINDOW_H
#define EDITWORKSWINDOW_H

#include <QDialog>
#include <QDebug>

#include "db.h"
#include "worksmodel.h"
#include "creatework.h"

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
private slots:
    void on_backButton_clicked();
    void on_addButton_clicked();
    void on_delButton_clicked();

private:

    void createUI();

private:
    Ui::EditWorksWindow *ui;
    DataBase *db;
    WorksModel *model;
    CreateWork *newWork;
};

#endif // EDITWORKSWINDOW_H
