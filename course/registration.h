#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "custombox.h"
#include <QRegularExpressionValidator>
#include <QDialog>
#include <QDebug>
#include "db.h"
#include <memory>

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
    std::shared_ptr <DataBase> db;
    QRegularExpressionValidator phoneValidator;
    QRegularExpressionValidator textValidator;
private:
    bool validateFields();
    void validatePassword(QString pswd);
    QVariantList getUserData();
};

#endif // REGISTRATION_H
