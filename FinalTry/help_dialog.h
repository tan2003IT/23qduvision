#ifndef HELP_DIALOG_H
#define HELP_DIALOG_H

#include <QDialog>

namespace Ui {
class Help_Dialog;
}

class Help_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Help_Dialog(QWidget *parent = nullptr);
    ~Help_Dialog();

private:
    Ui::Help_Dialog *ui;
};

#endif // HELP_DIALOG_H
