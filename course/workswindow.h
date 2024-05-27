#ifndef WORKSWINDOW_H
#define WORKSWINDOW_H

#include <QDialog>
#include <QDebug>
#include "customproxymodel.h"

#include "worksmodel.h"
#include "descwork.h"

namespace Ui {
class WorksWindow;
}

class WorksWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WorksWindow(QString currentLogin, QWidget *parent = nullptr);
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
    QSortFilterProxyModel *proxyModel;
private:
    const QString login;
};

#endif // WORKSWINDOW_H
