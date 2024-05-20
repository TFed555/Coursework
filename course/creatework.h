#ifndef CREATEWORK_H
#define CREATEWORK_H

#include <QSqlQuery>
#include <QDialog>
#include <QRegExpValidator>
#include "db.h"

namespace Ui {
class CreateWork;
}

class CreateWork : public QDialog
{
    Q_OBJECT

public:
    explicit CreateWork(QWidget *parent = nullptr);
    ~CreateWork();

private slots:
    void on_confirmButton_clicked();
    void on_backButton_clicked();

signals:
    void MainWindow();
private:
    void setComboBox();
private:
    Ui::CreateWork *ui;
    DataBase *db;
    QIntValidator payValidator;
    QRegExpValidator textValidator;
};

#endif // CREATEWORK_H
