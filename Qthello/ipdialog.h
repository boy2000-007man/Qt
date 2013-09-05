#ifndef IPDIALOG_H
#define IPDIALOG_H

#include <QDialog>
#include "ipconverter.h"
namespace Ui {
class IPDialog;
}

class IPDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit IPDialog(QWidget *parent = 0);
    ~IPDialog();
    IPconverter *converter;
private:
    Ui::IPDialog *ui;
};

#endif // IPDIALOG_H
