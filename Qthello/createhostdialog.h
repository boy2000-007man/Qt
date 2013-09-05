#ifndef CREATEHOSTDIALOG_H
#define CREATEHOSTDIALOG_H

#include <QDialog>

class createHostDialog : public QDialog
{
    Q_OBJECT
public:
    explicit createHostDialog(QWidget *parent = 0);
    QString hostIp;
signals:
    
public slots:
    
};

#endif // CREATEHOSTDIALOG_H
