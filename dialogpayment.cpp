#include <QMessageBox>
#include <QtSql>

#include <QDebug>

#include "dialogpayment.h"
#include "ui_dialogpayment.h"

DialogPayment::DialogPayment(QWidget *parent, int cur_id) :
    QDialog(parent),
    ui(new Ui::DialogPayment),
    current_id(cur_id)
{
    ui->setupUi(this);
    model = new QSqlTableModel();
    model->setTable("Payments");
    model->select();

    contracts_model = new QSqlTableModel();
    contracts_model->setTable("SupplyContracts");
    contracts_model->select();

    ui->comboBoxContractSupply->setModel(contracts_model);
    ui->comboBoxContractSupply->setModelColumn(0);

    ui->lineEditId->setText("Номер заполняется автоматически.");
    ui->dateEditDate->setDate(QDate::currentDate());

    QRegExp rx("[1-9]\\d*(\\.\\d{0,2})?");
    ui->lineEditTotal_price->setValidator(new QRegExpValidator(rx, this));

    if(cur_id != -1){
        QSqlQuery query;
        query.exec(QString("SELECT * FROM Payments WHERE id = %1").arg(current_id));
        if( query.next() ) {
            ui->lineEditId->setText(query.value("id").toString());
            ui->dateEditDate->setDate(QDate::fromString(query.value("date").toString(),"yyyy-MM-dd"));
            ui->lineEditTotal_price->setText(query.value("total_price").toString());
            ui->checkBoxPaid->setChecked(query.value("paid").toBool());

            int contract_id = query.value("contract").toInt();

            QModelIndexList il = contracts_model->match(contracts_model->index(0,0),0,contract_id);
            int cur_index = 0;
            if(il.size()>0){
                cur_index = il.at(0).row();
            }
            ui->comboBoxContractSupply->setCurrentIndex(cur_index);
        }
    }
    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &DialogPayment::finishOnAccept);
}
void DialogPayment::finishOnAccept()
{
    QMessageBox msg;
    if(current_id == -1){
        msg.setText("Добавить новое платежное поручение?");
    } else {
        msg.setText("Вы уверены, что хотите изменить поручение?");
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
        msg.setText("Ошибка, не удалось найти платежное поручение для изменения.");
        msg.setInformativeText("Добавить как новое?");
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
    QModelIndex ci = contracts_model->index(ui->comboBoxContractSupply->currentIndex(),0);
    model->setData(model->index(itemRow, 1), contracts_model->data(ci).toInt());
    model->setData(model->index(itemRow, 2), ui->lineEditTotal_price->text().toDouble());
    model->setData(model->index(itemRow, 3), ui->checkBoxPaid->isChecked()?"true":"false");
    model->setData(model->index(itemRow, 4), ui->dateEditDate->date().toString("yyyy-MM-dd"));

    model->submitAll();

    accept();
}
DialogPayment::~DialogPayment()
{
    delete ui;
    delete model;
    delete contracts_model;
}
