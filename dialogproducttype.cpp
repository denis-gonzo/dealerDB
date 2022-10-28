#include <QMessageBox>
#include <QtSql>

#include <QDebug>

#include "dialogproducttype.h"
#include "ui_dialogproducttype.h"

DialogProductType::DialogProductType(QWidget *parent, int cur_id) :
    QDialog(parent),
    ui(new Ui::DialogProductType),
    current_id(cur_id)
{
    ui->setupUi(this);

    model = new QSqlTableModel();
    model->setTable("ProductTypes");
    model->select();

    ui->lineEditId->setText("Номер заполняется автоматически.");

    if(cur_id != -1){
        QSqlQuery query;
        query.exec(QString("SELECT * FROM ProductTypes WHERE id = %1").arg(current_id));
        if( query.next() ) {
            ui->lineEditId->setText(query.value("id").toString());
            ui->lineEditName->setText(query.value("name").toString());
        }
    }
    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &DialogProductType::finishOnAccept);
}
void DialogProductType::finishOnAccept()
{
    QMessageBox msg;
    if(current_id == -1){
        msg.setText("Добавить новый тип продуктов?");
    } else {
        msg.setText("Вы уверены, что хотите изменить тип продуктов?");
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
        msg.setText("Ошибка, не удалось найти тип продуктов для изменения.");
        msg.setInformativeText("Добавить тип как новый?");
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

    model->submitAll();

    accept();
}
DialogProductType::~DialogProductType()
{
    delete ui;
    delete model;
}
