#ifndef WORKSWINDOW_H
#define WORKSWINDOW_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QDebug>

#include "db.h"
#include "worksmodel.h"
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

signals:
    void AuthoWindow();
private:
    void createUI();
private slots:
    void showWork(const QModelIndex &index);
    void on_backButton_clicked();

private:
    Ui::WorksWindow *ui;
    DescWork *itemUi;
    WorksModel *mymodel;
};

#endif // WORKSWINDOW_H
