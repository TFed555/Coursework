#ifndef WORKSWINDOW_H
#define WORKSWINDOW_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QDebug>

#include "db.h"

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

private:
    Ui::WorksWindow *ui;
    DataBase *db;
    QSqlQueryModel *model;
};

#endif // WORKSWINDOW_H
