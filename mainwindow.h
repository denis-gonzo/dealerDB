#pragma once

#include <QMainWindow>
#include <QSqlDatabase>

class PriceDelegate;
class QItemDelegate;

class QSqlQueryModel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpenDB_triggered();
    void on_actionCreateDB_triggered();

    void on_actionVendors_triggered();
    void on_actionClients_triggered();
    void on_actionProducts_triggered();
    void on_actionProductTypes_triggered();
    void on_actionContractsSale_triggered();
    void on_actionChecks_triggered();
    void on_actionContractsSupply_triggered();
    void on_actionPayments_triggered();
    void on_actionProductsSale_triggered();
    void on_actionProductsSupply_triggered();

    void on_actionQueryProducts_triggered();
    void on_actionQueryClients_triggered();
    void on_actionQueryPotentialClients_triggered();
    void on_actionQuerySaleSupplyDone_triggered();
    void on_actionQueryAllProducts_triggered();
    void on_actionQueryPaid_triggered();
    void on_actionQueryUnpaid_triggered();
    void on_actionQuerySaleSupplyWait_triggered();
    void on_actionQueryContracts_triggered();

    void on_pushButtonUpdate_clicked();
    void on_pushButtonPrint_clicked();

private:
    Ui::MainWindow *ui;
    PriceDelegate *pd;
    QItemDelegate *def_dlg;
    QString db_name;
    QSqlDatabase db;
    QSqlQueryModel *p_mdl;
    QString query_title;

    bool CreateConnection();
    bool CheckDatabase();
};
