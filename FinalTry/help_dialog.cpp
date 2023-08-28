#include "help_dialog.h"
#include "ui_help_dialog.h"

Help_Dialog::Help_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Help_Dialog)
{
    ui->setupUi(this);
}

Help_Dialog::~Help_Dialog()
{
    delete ui;
}
