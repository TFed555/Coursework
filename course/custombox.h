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
    void showWarningBox(const QString &msg){
        QMessageBox::StandardButton reply;
          setFixedSize(width, height);
          setSizeGripEnabled(true);
          reply = QMessageBox::warning(this, "Предупреждение", msg);
          if (reply == QMessageBox::Yes) {
            qDebug() << "Yes was clicked";
            //QApplication::quit();
          } else {
            qDebug() << "Yes was *not* clicked";
          }
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
