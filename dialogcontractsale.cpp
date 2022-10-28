#include <QMessageBox>
#include <QtSql>

#include <QDebug>

#include "dialogcontractsale.h"
#include "ui_dialogcontractsale.h"

DialogContractSale::DialogContractSale(QWidget *parent, int cur_id) :
    QDialog(parent),
    ui(new Ui::DialogContractSale),
    current_id(cur_id)
{
    ui->setupUi(this);
    model = new QSqlTableModel();
    model->setTable("SaleContracts");
    model->select();

    clients_model = new QSqlTableModel();
    clients_model->setTable("Clients");
    clients_model->select();

    ui->comboBoxClient->setModel(clients_model);
    ui->comboBoxClient->setModelColumn(1);

    ui->lineEditId->setText("Номер заполняется автоматически.");
    ui->dateEditDate->setDate(QDate::currentDate());

    if(cur_id != -1){
        QSqlQuery query;
        query.exec(QString("SELECT * FROM SaleContracts WHERE id = %1").arg(current_id));
        if( query.next() ) {
            ui->lineEditId->setText(query.value("id").toString());
            ui->dateEditDate->setDate(QDate::fromString(query.value("date").toString(),"yyyy-MM-dd"));
            ui->textEditText->setText(query.value("text").toString());

            int client_id = query.value("client").toInt();

            QModelIndexList il = clients_model->match(clients_model->index(0,0),0,client_id);
            int cur_index = 0;
            if(il.size()>0){
                cur_index = il.at(0).row();
            }
            ui->comboBoxClient->setCurrentIndex(cur_index);
        }
    }
    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &DialogContractSale::finishOnAccept);
}
void DialogContractSale::finishOnAccept()
{
    QMessageBox msg;
    if(current_id == -1){
        msg.setText("Добавить новый договор продажи?");
    } else {
        msg.setText("Вы уверены, что хотите изменить договор?");
    }
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msg.exec();
    if( ret != QMessageBox::Ok) {
        return;
    }

    int itemRow;

    QModelIndexList il = model->match(model->index(0,0),0,current_id);
    if( current_id != -1 && il.size() < 1 ){
        qDebug() << "Error searching id";
        msg.setText("Ошибка, не удалось найти договор для изменения.");
        msg.setInformativeText("Добавить договор как новый?");
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
    QModelIndex ci = clients_model->index(ui->comboBoxClient->currentIndex(),0);
    model->setData(model->index(itemRow, 1), ui->dateEditDate->date().toString("yyyy-MM-dd"));
    model->setData(model->index(itemRow, 2), ui->textEditText->toPlainText());
    model->setData(model->index(itemRow, 3), clients_model->data(ci).toInt());

    model->submitAll();

    accept();
}
DialogContractSale::~DialogContractSale()
{
    delete ui;
    delete model;
    delete clients_model;
}
