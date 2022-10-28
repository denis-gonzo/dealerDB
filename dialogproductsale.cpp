#include <QMessageBox>
#include <QtSql>

#include <QDebug>

#include "dialogproductsale.h"
#include "ui_dialogproductsale.h"

DialogProductSale::DialogProductSale(QWidget *parent, int cur_id) :
    QDialog(parent),
    ui(new Ui::DialogProductSale),
    current_id(cur_id)
{
    ui->setupUi(this);
    model = new QSqlTableModel();
    model->setTable("ProductsSale");
    model->select();

    contracts_model = new QSqlTableModel();
    contracts_model->setTable("SaleContracts");
    contracts_model->select();

    products_model = new QSqlTableModel();
    products_model->setTable("Products");
    products_model->select();

    ui->comboBoxContractSale->setModel(contracts_model);
    ui->comboBoxContractSale->setModelColumn(0);

    ui->comboBoxProduct->setModel(products_model);
    ui->comboBoxProduct->setModelColumn(2);

    if(cur_id != -1){
        QSqlQuery query;
        query.exec(QString("SELECT * FROM ProductsSale WHERE id = %1").arg(current_id));
        if( query.next() ) {
            int product_id  = query.value("product").toInt();
            QModelIndexList il = products_model->match(products_model->index(0,0),0,product_id);
            int cur_index = 0;
            if(il.size()>0){
                cur_index = il.at(0).row();
            }
            ui->comboBoxProduct->setCurrentIndex(cur_index);
            ui->lineEditCount->setText(query.value("count").toString());

            int contract_id = query.value("contract").toInt();
            il = contracts_model->match(contracts_model->index(0,0),0,contract_id);
            if(il.size()>0){
                cur_index = il.at(0).row();
            }
            ui->comboBoxContractSale->setCurrentIndex(cur_index);
            ui->checkBoxShipped->setChecked(query.value("shipped").toBool());

        }
    }
    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &DialogProductSale::finishOnAccept);
}
void DialogProductSale::finishOnAccept()
{
    QMessageBox msg;
    if(current_id == -1){
        msg.setText("Добавить новую продажу товара?");
    } else {
        msg.setText("Вы уверены, что хотите изменить продажу?");
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
        msg.setText("Ошибка, не удалось найти продажу для изменения.");
        msg.setInformativeText("Добавить как новую?");
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
    QModelIndex ci = products_model->index(ui->comboBoxProduct->currentIndex(),0);
    model->setData(model->index(itemRow, 1), products_model->data(ci).toInt());
    model->setData(model->index(itemRow, 2), ui->lineEditCount->text().toInt());
    ci = contracts_model->index(ui->comboBoxContractSale->currentIndex(),0);
    model->setData(model->index(itemRow, 3), contracts_model->data(ci).toInt());
    model->setData(model->index(itemRow, 4), ui->checkBoxShipped->isChecked()?"true":"false");

    model->submitAll();

    accept();
}
DialogProductSale::~DialogProductSale()
{
    delete ui;
    delete model;
    delete products_model;
    delete contracts_model;
}
