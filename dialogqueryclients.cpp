#include <QtSql>

#include "dialogqueryclients.h"
#include "ui_dialogqueryclients.h"

DialogQueryClients::DialogQueryClients(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogQueryClients)
{
    ui->setupUi(this);

    mdl = new QSqlTableModel();
    mdl->setTable("ProductTypes");
    mdl->select();

    ui->comboBox->setModel(mdl);
    ui->comboBox->setModelColumn(1);

}
int DialogQueryClients::getType()
{
    QModelIndex ci = mdl->index(ui->comboBox->currentIndex(),0);
    return mdl->data(ci).toInt();
}
DialogQueryClients::~DialogQueryClients()
{
    delete ui;
    delete mdl;
}
