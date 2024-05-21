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
        box.setWindowTitle("Предупреждение");
        box.setText(msg);
        box.setIcon(QMessageBox::Warning);
        int reply = box.exec();
        return reply;
    }
    void showErrorBox(const QString &msg){
        QMessageBox::StandardButton reply;
          setFixedSize(width, height);
          setSizeGripEnabled(true);
          reply = QMessageBox::critical(this, "Ошибка", msg);
          if (reply == QMessageBox::Yes) {
            qDebug() << "Yes was clicked";
            //QApplication::quit();
          } else {
            qDebug() << "Yes was *not* clicked";
          }
    }
};


#endif // CUSTOMBOX_H
