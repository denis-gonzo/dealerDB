#pragma once

#include <QDialog>

class PriceDelegate;
class QSqlRelationalTableModel;

namespace Ui {
class DialogTableDB;
}

class DialogTableDB : public QDialog
{
    Q_OBJECT

public:
    enum TableName {
        Vendors,
        Clients,
        Products,
        ProductTypes,
        SaleContracts,
        Checks,
        ProductsSale,
        SupplyContracts,
        Payments,
        ProductsSupply
    };

    explicit DialogTableDB(QWidget *parent = nullptr, TableName table = Products );
    ~DialogTableDB();

private slots:
    void on_pushButtonAdd_clicked();
    void on_pushButtonEdit_clicked();
    void on_pushButtonDelete_clicked();

private:
    Ui::DialogTableDB *ui;
    PriceDelegate* pd;
    QSqlRelationalTableModel *p_mdl;
    TableName current_table;

    void dialogExec(int cur_id = -1);

    void setupVendors();
    void setupClients();
    void setupProducts();
    void setupProductTypes();
    void setupSaleContracts();
    void setupChecks();
    void setupProductsSale();
    void setupSupplyContracts();
    void setupPayments();
    void setupProductsSupply();

};

