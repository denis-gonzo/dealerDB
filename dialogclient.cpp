#include <QMessageBox>
#include <QtSql>

#include <QDebug>

#include "dialogclient.h"
#include "ui_dialogclient.h"

DialogClient::DialogClient(QWidget *parent, int cur_id) :
    QDialog(parent),
    ui(new Ui::DialogClient),
    current_id(cur_id)
{
    ui->setupUi(this);
    model = new QSqlTableModel();
    model->setTable("Clients");
    model->select();
    if(cur_id != -1){
        QSqlQuery query;
        query.exec(QString("SELECT * FROM Clients WHERE id = %1").arg(current_id));
        if( query.next() ) {
            //ui->lineEditId->setText(query.value("id").toString());
            ui->lineEditName->setText(query.value("name").toString());
            ui->lineEditAddress->setText(query.value("address").toString());
            ui->lineEditPhone->setText(query.value("phone").toString());
            ui->lineEditFax->setText(query.value("fax").toString());
            ui->lineEditAccount->setText(query.value("account").toString());
            ui->lineEditNotes->setText(query.value("notes").toString());
        }
    }
    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &DialogClient::finishOnAccept);
}
void DialogClient::finishOnAccept()
{
    QMessageBox msg;
    if(current_id == -1){
        msg.setText("Добавить клиента?");
    } else {
        msg.setText("Вы уверены, что хотите изменить данные клиента?");
    }
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msg.exec();
    if( ret != QMessageBox::Ok) {
        return;
    }

    int itemRow;

    QModelIndexList il = model->match(model->index(0,0),0,current_id);
    if( current_id != -1 && il.size() < 1 ){
        qDebug() << "Error searching id in ProductTypes";
        msg.setText("Ошибка, не удалось найти клиента для изменения.");
        msg.setInformativeText("Добавить нового?");
        int ret = msg.exec();
        if( ret != QMessageBox::Ok) {
            return;
        }
    }

    if( current_id == -1 || il.size() < 1 ) //Add new
    {
        if( !model->insertRows(model->rowCount(),1) ){
            qDebug() << "Error on insert row!";
        }
        itemRow = model->rowCount()-1;
    } else {
        itemRow = il.last().row();
    }

    model->setData(model->index(itemRow, 1), ui->lineEditName->text());
    model->setData(model->index(itemRow, 2), ui->lineEditAddress->text());
    model->setData(model->index(itemRow, 3), ui->lineEditPhone->text());
    model->setData(model->index(itemRow, 4), ui->lineEditFax->text());
    model->setData(model->index(itemRow, 5), ui->lineEditAccount->text());
    model->setData(model->index(itemRow, 6), ui->lineEditNotes->text());

    model->submitAll();

    accept();
}
DialogClient::~DialogClient()
{
    delete ui;
    delete model;
}
