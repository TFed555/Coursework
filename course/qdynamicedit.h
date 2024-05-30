#ifndef QDYNAMICEDIT_H
#define QDYNAMICEDIT_H

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class QDynamicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDynamicWidget(QWidget *parent = 0);
        ~QDynamicWidget();
        static int ResID;
        int getID();

public slots:

public:
    QLabel* createLabel(const QString& text);
    QLineEdit* createEdit(const QString& text);
    QPushButton* createButton(const QString& text);

private:
        int ID = 0;
};




#endif // QDYNAMICEDIT_H
