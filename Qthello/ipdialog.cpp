#include "ipdialog.h"
#include "ui_ipdialog.h"
#include <QGridLayout>
#include <QSignalMapper>
#include <QLabel>
#include <QLineEdit>
#include <QtGui>
#include <QPushButton>
IPDialog::IPDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IPDialog)
{
    ui->setupUi(this);

    QLabel *label = new QLabel("Host IP: ", this);
    QLineEdit *lineEdit = new QLineEdit(this);
    lineEdit->setReadOnly(true);
    lineEdit->setAlignment(Qt::AlignRight);
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->addWidget(label, 0, 0);
    gridLayout->addWidget(lineEdit, 0, 1, 1, 4);

    QSignalMapper *mapper = new QSignalMapper(this);
    for (int i = 0; i < 10; i++) {
        QPushButton *p = new QPushButton(QString("%1").arg(i), this);
        gridLayout->addWidget(p, 1+i/5, i%5);
        QObject::connect(p, SIGNAL(clicked()), mapper, SLOT(map()));
        mapper->setMapping(p, i);
    }
    QPushButton *dot = new QPushButton(QString("."), this);
    gridLayout->addWidget(dot, 3, 0);
    QObject::connect(dot, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(dot, -2);
    QPushButton *del = new QPushButton(QString("delete"), this);
    gridLayout->addWidget(del, 3, 1);
    QObject::connect(del, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(del, -1);

    QPushButton *ok = new QPushButton(QString("ok"), this);
    gridLayout->addWidget(ok, 3, 2, 1, 3);
    QObject::connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
    converter = new IPconverter(this);
    QObject::connect(mapper, SIGNAL(mapped(int)), converter, SLOT(addNumber(int)));
    QObject::connect(converter, SIGNAL(changed(QString)), lineEdit, SLOT(setText(QString)));
}

IPDialog::~IPDialog()
{
    delete ui;
}
