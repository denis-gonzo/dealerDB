#include <QMessageBox>
#include <QtSql>

#include <QDebug>

#include "dialogproduct.h"
#include "ui_dialogproduct.h"

DialogProduct::DialogProduct(QWidget *parent, int cur_id) :
    QDialog(parent),
    ui(new Ui::DialogProduct),
    current_id(cur_id)
{
    ui->setupUi(this);

    QRegExp rx("[1-9]\\d*(\\.\\d{0,2})?");
    ui->lineEditPrice->setValidator(new QRegExpValidator(rx, this));

    auto iv = new QIntValidator();
    iv->setBottom(0);
    ui->lineEditCount->setValidator(iv);

    model = new QSqlTableModel();
    model->setTable("Products");
    model->select();

    types_model = new QSqlTableModel();
    types_model->setTable("ProductTypes");
    types_model->select();

    ui->comboBoxType->setModel(types_model);
    ui->comboBoxType->setModelColumn(1);

    if(cur_id != -1){
        QSqlQuery query;
        query.exec(QString("SELECT * FROM Products WHERE id = %1").arg(current_id));
        if( query.next() ) {
            ui->lineEditName->setText(query.value("name").toString());
            ui->lineEditProps->setText(query.value("properties").toString());
            ui->lineEditPrice->setText(query.value("price").toString());
            ui->lineEditPackage->setText(query.value("package").toString());
            ui->lineEditSupply->setText(query.value("supply").toString());
            ui->lineEditCount->setText(query.value("count").toString());
            int type_id = query.value("type").toInt();

            QModelIndexList il = types_model->match(types_model->index(0,0),0,type_id);
            int cur_index = 0;
            if(il.size()>0){
                cur_index = il.at(0).row();
            }
            ui->comboBoxType->setCurrentIndex(cur_index);
        }
    }
    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &DialogProduct::finishOnAccept);
}

void DialogProduct::finishOnAccept()
{
    QMessageBox msg;
    if(current_id == -1){
        msg.setText("Добавить новый продукт?");
    } else {
        msg.setText("Вы уверены, что хотите изменить продукт?");
    }
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msg.exec();
    if( ret != QMessageBox::Ok) {
        return;
    }

    int itemRow;

    QModelIndexList il = model->match(model->index(0,0),0,current_id);
    if( current_id != -1 && il.size() < 1 ){
        qDebug() << "Error searching id in Products";
        msg.setText("Ошибка, не удалось найти продукт для изменения.");
        msg.setInformativeText("Добавить продукт как новый?");
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
    QModelIndex ci = types_model->index(ui->comboBoxType->currentIndex(),0);
    model->setData(model->index(itemRow, 1), types_model->data(ci).toInt());
    model->setData(model->index(itemRow, 2), ui->lineEditName->text());
    model->setData(model->index(itemRow, 3), ui->lineEditProps->text());
    model->setData(model->index(itemRow, 4), ui->lineEditPrice->text().toDouble());
    model->setData(model->index(itemRow, 5), ui->lineEditPackage->text());
    model->setData(model->index(itemRow, 6), ui->lineEditSupply->text());
    model->setData(model->index(itemRow, 7), ui->lineEditCount->text().toInt());
    model->submitAll();

    accept();
}
DialogProduct::~DialogProduct()
{
    delete ui;
    delete model;
    delete types_model;
}
