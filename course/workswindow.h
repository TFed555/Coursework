#ifndef WORKSWINDOW_H
#define WORKSWINDOW_H

#include <QDialog>
#include <QDebug>
#include "worksproxymodel.h"

#include "db.h"
#include "worksmodel.h"
#include "descwork.h"
#include <memory>

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
    void WorkerWindow();
private:
    void createUI();
private slots:
    void showWork(const QModelIndex &index);
    void on_backButton_clicked();

private:
    Ui::WorksWindow *ui;
    std::shared_ptr <DescWork> workUi;
    WorksModel *mymodel;
    std::shared_ptr <WorksSortFilterProxyModel> proxyModel;
    std::shared_ptr <DataBase> db;
private:
    const QString login;
};

#endif // WORKSWINDOW_H
