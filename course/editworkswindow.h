#ifndef EDITWORKSWINDOW_H
#define EDITWORKSWINDOW_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QDebug>

#include "db.h"


namespace Ui {
class EditWorksWindow;
}

class EditWorksWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EditWorksWindow(QWidget *parent = nullptr);
    ~EditWorksWindow();


private:
    void setupModel(const QStringList &headers);
    void createUI();

private:
    Ui::EditWorksWindow *ui;
    DataBase *db;
    QSqlQueryModel *model;
};

#endif // EDITWORKSWINDOW_H
