#include "qdynamicedit.h"

QDynamicEdit::QDynamicEdit(QWidget *parent) :
    QLineEdit(parent)
{
    ResID++;
    ID = ResID;

}

QDynamicEdit::~QDynamicEdit()
{

}


int QDynamicEdit::getID()
{
    return ID;
}


int QDynamicEdit::ResID = 0;
