#include <QtSql>
#include <QMessageBox>

#include <QDebug>

#include "dialogvendor.h"
#include "ui_dialogvendor.h"

DialogVendor::DialogVendor(QWidget *parent, int cur_id) :
    QDialog(parent),
    ui(new Ui::DialogVendor),
    current_id(cur_id)
{
    ui->setupUi(this);
    model = new QSqlTableModel();
    model->setTable("Vendors");
    model->select();
    if(cur_id != -1){
        QSqlQuery query;
        query.exec(QString("SELECT * FROM Vendors WHERE id = %1").arg(current_id));
        if( query.next() ) {
            //ui->lineEditI->setText(query.value("id").toString());
            ui->lineEditName->setText(query.value("name").toString());
            ui->lineEditAddress->setText(query.value("address").toString());
            ui->lineEditManager->setText(query.value("manager").toString());
            ui->lineEditAccountant->setText(query.value("accountant").toString());
            ui->lineEditAccount_info->setText(query.value("account_info").toString());
        }
    }
    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &DialogVendor::finishOnAccept);
}
void DialogVendor::finishOnAccept()
{
    QMessageBox msg;
    if(current_id == -1){
        msg.setText("Добавить производителя?");
    } else {
        msg.setText("Вы уверены, что хотите изменить данные производителя?");
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
        msg.setText("Ошибка, не удалось найти производителя для изменения.");
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
    model->setData(model->index(itemRow, 3), ui->lineEditManager->text());
    model->setData(model->index(itemRow, 4), ui->lineEditAccountant->text());
    model->setData(model->index(itemRow, 5), ui->lineEditAccount_info->text());

    model->submitAll();

    accept();
}
DialogVendor::~DialogVendor()
{
    delete ui;
    delete model;
}
