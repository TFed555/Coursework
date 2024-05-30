#include "qdynamicedit.h"

QDynamicWidget::QDynamicWidget(QWidget *parent) :
    QWidget(parent)
{
    ResID++;
    ID = ResID;

}

QDynamicWidget::~QDynamicWidget()
{

}


int QDynamicWidget::getID()
{
    return ID;
}


int QDynamicWidget::ResID = 0;

QLabel* QDynamicWidget::createLabel(const QString& text) {
    QLabel* label = new QLabel(this);
    label->setText(text);
    return label;
}

QLineEdit* QDynamicWidget::createEdit(const QString& text) {
    QLineEdit* edit = new QLineEdit(this);
    edit->setText(text);
    return edit;
}

QPushButton* QDynamicWidget::createButton(const QString& text) {
    QPushButton* button = new QPushButton(this);
    button->setText(text);
    return button;
}
