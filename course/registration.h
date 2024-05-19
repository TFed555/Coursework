#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "custombox.h"
#include <QRegExpValidator>
#include <QDialog>
#include <QDebug>
#include "db.h"

namespace Ui {
class Registration;
}

class Registration : public QDialog
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr);
    ~Registration();

signals:
   void AuthoWindow();
   void UpdateData();
   void clearValid();

private slots:

   void clearFields();

   bool on_confirmButton_clicked();

   void on_backButton_clicked();

   void on_pswdEdit_textChanged(const QString &arg1);

private:
    Ui::Registration *ui;
    DataBase *db;
    QRegExpValidator phoneValidator;
    QRegExpValidator textValidator;
    bool validateFields();
    void validatePassword(QString pswd);
};

#endif // REGISTRATION_H
