#include <QtSql>
#include <QMessageBox>

#include <QDebug>

#include "dialogtabledb.h"
#include "ui_dialogtabledb.h"

#include "customtabledelegate.h"
#include "dialogclient.h"
#include "dialogvendor.h"
#include "dialogproduct.h"
#include "dialogproducttype.h"
#include "dialogcontractsale.h"
#include "dialogcontractsupply.h"
#include "dialogcheck.h"
#include "dialogpayment.h"
#include "dialogproductsale.h"
#include "dialogproductsupply.h"

DialogTableDB::DialogTableDB(QWidget *parent, TableName table) :
    QDialog(parent),
    ui(new Ui::DialogTableDB),
    current_table(table)
{
    ui->setupUi(this);

    pd = new PriceDelegate(ui->tableView);

    p_mdl = new QSqlRelationalTableModel();

    ui->tableView->setModel(p_mdl);

    switch (current_table) {
    case Vendors:
        setupVendors();
        break;
    case Clients:
        setupClients();
        break;
    case Products:
        setupProducts();
        break;
    case ProductTypes:
        setupProductTypes();
        break;
    case SaleContracts:
        setupSaleContracts();
        break;
    case Checks:
        setupChecks();
        break;
    case ProductsSale:
        setupProductsSale();
        break;
    case SupplyContracts:
        setupSupplyContracts();
        break;
    case Payments:
        setupPayments();
        break;
    case ProductsSupply:
        setupProductsSupply();
        break;
    default:
        break;
    }

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    p_mdl->select();
    p_mdl->setEditStrategy(QSqlTableModel::OnManualSubmit);

    if( p_mdl->rowCount() < 1 )
    {
        ui->pushButtonEdit->setEnabled(false);
        ui->pushButtonDelete->setEnabled(false);
    } else {
        ui->tableView->selectRow(0);
    }
    connect(ui->tableView, &QTableView::doubleClicked,
            this, &DialogTableDB::on_pushButtonEdit_clicked);
}
void DialogTableDB::setupVendors()
{
    setWindowTitle("Производители");
    p_mdl->setTable("Vendors");
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("id"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Наименование"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Адрес"));
    p_mdl->setHeaderData(3, Qt::Horizontal, tr("Директор"));
    p_mdl->setHeaderData(4, Qt::Horizontal, tr("Бухгалтер"));
    p_mdl->setHeaderData(5, Qt::Horizontal, tr("Реквизиты"));
    p_mdl->sort(1,Qt::AscendingOrder);

    ui->tableView->hideColumn(0);
}
void DialogTableDB::setupClients()
{
    setWindowTitle("Клиенты");
    p_mdl->setTable("Clients");
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("id"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Ф. И. О."));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Адрес"));
    p_mdl->setHeaderData(3, Qt::Horizontal, tr("Телефон"));
    p_mdl->setHeaderData(4, Qt::Horizontal, tr("Факс"));
    p_mdl->setHeaderData(5, Qt::Horizontal, tr("Счет"));
    p_mdl->setHeaderData(6, Qt::Horizontal, tr("Заметки"));
    p_mdl->sort(1,Qt::AscendingOrder);

    ui->tableView->hideColumn(0);
}

void DialogTableDB::setupProducts()
{
    setWindowTitle("Товары");
    p_mdl->setTable("Products");
    p_mdl->setRelation(1, QSqlRelation("ProductTypes", "id", "name"));
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("id"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Тип"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Наименование"));
    p_mdl->setHeaderData(3, Qt::Horizontal, tr("Свойства"));
    p_mdl->setHeaderData(4, Qt::Horizontal, tr("Цена"));
    p_mdl->setHeaderData(5, Qt::Horizontal, tr("Упаковка"));
    p_mdl->setHeaderData(6, Qt::Horizontal, tr("Поставка"));
    p_mdl->setHeaderData(7, Qt::Horizontal, tr("Количество"));
    p_mdl->sort(1,Qt::AscendingOrder);

    ui->tableView->setItemDelegateForColumn(4, pd);

    ui->tableView->hideColumn(0);
}
void DialogTableDB::setupProductTypes()
{
    setWindowTitle("Типы товаров");
    p_mdl->setTable("ProductTypes");
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("id"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Наименование"));

}
void DialogTableDB::setupSaleContracts()
{
    setWindowTitle("Договоры продаж");
    p_mdl->setTable("SaleContracts");
    p_mdl->setRelation(3, QSqlRelation("Clients", "id", "name"));
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("Номер"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Дата"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Текст"));
    p_mdl->setHeaderData(3, Qt::Horizontal, tr("Клиент"));
    p_mdl->sort(0,Qt::AscendingOrder);
}
void DialogTableDB::setupChecks()
{
    setWindowTitle("Счета продаж");
    p_mdl->setTable("Checks");
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("Номер"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Договор"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Сумма"));
    p_mdl->setHeaderData(3, Qt::Horizontal, tr("Оплачен"));
    p_mdl->setHeaderData(4, Qt::Horizontal, tr("Дата"));

    ui->tableView->setItemDelegateForColumn(2, pd);

    p_mdl->sort(0,Qt::AscendingOrder);
}
void DialogTableDB::setupProductsSale()
{
    setWindowTitle("Продажи товаров");
    p_mdl->setTable("ProductsSale");
    p_mdl->setRelation(1, QSqlRelation("Products", "id", "name"));
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("id"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Товар"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Количество"));
    p_mdl->setHeaderData(3, Qt::Horizontal, tr("Договор"));
    p_mdl->setHeaderData(4, Qt::Horizontal, tr("Отгружен"));
    p_mdl->sort(1,Qt::AscendingOrder);

    ui->tableView->hideColumn(0);
}
void DialogTableDB::setupSupplyContracts()
{
    setWindowTitle("Договоры поставок");
    p_mdl->setTable("SupplyContracts");
    p_mdl->setRelation(3, QSqlRelation("Vendors", "id", "name"));
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("Номер"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Дата"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Текст"));
    p_mdl->setHeaderData(3, Qt::Horizontal, tr("Производитель"));
    p_mdl->sort(0,Qt::AscendingOrder);
}
void DialogTableDB::setupPayments()
{
    setWindowTitle("Платежные поручения поставок");
    p_mdl->setTable("Payments");
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("Номер"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Договор"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Сумма"));
    p_mdl->setHeaderData(3, Qt::Horizontal, tr("Оплачен"));
    p_mdl->setHeaderData(4, Qt::Horizontal, tr("Дата"));

    ui->tableView->setItemDelegateForColumn(2, pd);

    p_mdl->sort(0,Qt::AscendingOrder);
}
void DialogTableDB::setupProductsSupply()
{
    setWindowTitle("Поставки товаров");
    p_mdl->setTable("ProductsSupply");
    p_mdl->setRelation(1, QSqlRelation("Products", "id", "name"));
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("id"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Товар"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Количество"));
    p_mdl->setHeaderData(3, Qt::Horizontal, tr("Договор"));
    p_mdl->setHeaderData(4, Qt::Horizontal, tr("Получен"));
    p_mdl->sort(1,Qt::AscendingOrder);

    ui->tableView->hideColumn(0);
}


void DialogTableDB::on_pushButtonDelete_clicked()
{
    QMessageBox msg;
    msg.setText("Действие приведет к удалению записи из таблицы.");
    msg.setInformativeText("Вы уверены, что хотите продолжить?");
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msg.exec();
    if( ret != QMessageBox::Ok) {
        return;
    }
    int row = ui->tableView->currentIndex().row();
    p_mdl->removeRows(row,1);
    p_mdl->submitAll();

    p_mdl->select();

    if(p_mdl->rowCount()<1){
        ui->pushButtonDelete->setEnabled(false);
        ui->pushButtonEdit->setEnabled(false);
    }

}

void DialogTableDB::on_pushButtonAdd_clicked()
{
    dialogExec();

    ui->tableView->selectRow(0);

    if(p_mdl->rowCount()>0){
        ui->pushButtonDelete->setEnabled(true);
        ui->pushButtonEdit->setEnabled(true);
    }
}

void DialogTableDB::on_pushButtonEdit_clicked()
{
    int row = ui->tableView->currentIndex().row();
    int id = p_mdl->data(ui->tableView->currentIndex().siblingAtColumn(0)).toInt();
    dialogExec(id);
    ui->tableView->selectRow(row);
}

void DialogTableDB::dialogExec(int cur_id)
{
    switch (current_table) {
    case Vendors:
    {
        DialogVendor dlg(this, cur_id);
        dlg.setModal(true);
        dlg.exec();
        break;
    }
    case Clients:
    {
        DialogClient dlg(this, cur_id);
        dlg.setModal(true);
        dlg.exec();
        break;
    }
    case Products:
    {
        DialogProduct dlg(this, cur_id);
        dlg.setModal(true);
        dlg.exec();
        break;
    }
    case ProductTypes:
    {
        DialogProductType dlg(this, cur_id);
        dlg.setModal(true);
        dlg.exec();
        break;
    }
    case SaleContracts:
    {
        DialogContractSale dlg(this, cur_id);
        dlg.setModal(true);
        dlg.exec();
        break;
    }
    case Checks:
    {
        DialogCheck dlg(this, cur_id);
        dlg.setModal(true);
        dlg.exec();
        break;
    }
    case ProductsSale:
    {
        DialogProductSale dlg(this, cur_id);
        dlg.setModal(true);
        dlg.exec();
        break;
    }
    case SupplyContracts:
    {
        DialogContractSupply dlg(this, cur_id);
        dlg.setModal(true);
        dlg.exec();
        break;
    }
    case Payments:
    {
        DialogPayment dlg(this, cur_id);
        dlg.setModal(true);
        dlg.exec();
        break;
    }
    case ProductsSupply:
    {
        DialogProductSupply dlg(this, cur_id);
        dlg.setModal(true);
        dlg.exec();
        break;
    }

    default:
        break;
    }

    p_mdl->select();
}

DialogTableDB::~DialogTableDB()
{
    delete ui;
    delete p_mdl;
}
