#ifndef WORKSWINDOW_H
#define WORKSWINDOW_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QDebug>

#include "db.h"
#include "descwork.h"

namespace Ui {
class WorksWindow;
}

class WorksWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WorksWindow(QWidget *parent = nullptr);
    ~WorksWindow();

private:
    void setupModel(const QStringList &headers);
    void createUI();
private slots:
    void showWork(const QModelIndex &index);
private:
    Ui::WorksWindow *ui;
    DataBase *db;
    QSqlQueryModel *model;
    DescWork *itemUi;
};

#endif // WORKSWINDOW_H
