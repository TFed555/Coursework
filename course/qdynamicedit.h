#ifndef QDYNAMICEDIT_H
#define QDYNAMICEDIT_H

#include <QLineEdit>

class QDynamicEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QDynamicEdit(QWidget *parent = 0);
        ~QDynamicEdit();
        static int ResID;
        int getID();

public slots:

private:
        int ID = 0;
};




#endif // QDYNAMICEDIT_H
