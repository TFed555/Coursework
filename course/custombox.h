#ifndef CUSTOMBOX_H
#define CUSTOMBOX_H

#include <QMessageBox>
#include <QDebug>

class CustomBox : public QMessageBox
{
public:
    CustomBox(QWidget *parent = nullptr)
        : QMessageBox(parent)
    {

    }

public:
    const int width = 400, height = 200;
    int showWarningBox(const QString &msg){
        QMessageBox box;
        box.setFixedSize(width, height);
        box.setSizeGripEnabled(true);
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.setDefaultButton(QMessageBox::Cancel);
        box.setWindowTitle("Предупреждение");
        box.setText(msg);
        box.setIcon(QMessageBox::Warning);
//        box.setWindowModality(Qt::NonModal);
        //box.show();
        int reply = box.exec();
        return reply;
    }
    int showErrorBox(const QString &msg){
        QMessageBox box;
        box.setFixedSize(width, height);
        box.setSizeGripEnabled(true);
        box.setStandardButtons(QMessageBox::Ok);
        box.setWindowTitle("Ошибка");
        box.setText(msg);
        box.setIcon(QMessageBox::Critical);
        int reply = box.exec();
        return reply;
    }
};


#endif // CUSTOMBOX_H
