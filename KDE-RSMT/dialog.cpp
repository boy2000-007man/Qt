#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}
double Dialog::getX() const {
    return ui->x->value();
}
double Dialog::getY() const {
    return ui->y->value();
}
