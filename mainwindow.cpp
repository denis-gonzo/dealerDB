#include <QtSql>
#include <QMessageBox>
#include <QFileDialog>

#include <QTextDocument>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QPrinter>
#include <QPrintDialog>

#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogtabledb.h"
#include "dialogqueryclients.h"
#include "customtabledelegate.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    db = QSqlDatabase::addDatabase("QSQLITE"); //Драйвер SQLite
    p_mdl = new QSqlQueryModel();

    ui->setupUi(this);

    pd = new PriceDelegate(ui->tableView);
    def_dlg = new QItemDelegate();

    ui->menuDB->setEnabled(false);
    ui->menuQuery->setEnabled(false);
    ui->tableView->setModel(p_mdl);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete p_mdl;
    if(db.isOpen()){
        db.commit();
        db.close();
    }
}

bool MainWindow::CreateConnection()
{
    db.setDatabaseName(db_name); //Имя БД
    if (!db.open()) {            //Создание связи
        QMessageBox msgBox;
        QString str=QString("Ошибка при открытии базы данных:");
        msgBox.setText(str);
        msgBox.setWindowTitle("Ошибка");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setInformativeText(db.lastError().text());
        msgBox.exec();           //При ошибке отображение сообщения
        return false;
    }

    return true;
}
bool MainWindow::CheckDatabase()
{
    QStringList db_tables = db.tables();
    QStringList tables;
    tables << "Vendors"
           << "Clients"
           << "Products"
           << "ProductTypes"
           << "SaleContracts"
           << "Checks"
           << "ProductsSale"
           << "SupplyContracts"
           << "Payments"
           << "ProductsSupply";
    for(const auto& tab : tables){
        if(! db_tables.contains(tab)){
            QMessageBox msgBox;
            QString str=QString("Ошибка в базе данных:");
            msgBox.setText(str);
            msgBox.setWindowTitle("Ошибка");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setInformativeText("База данных не содержит необходимую структуру таблиц.");
            msgBox.exec(); //При ошибке отображение сообщения

            return false;
        }
    }
    return true;
}

void MainWindow::on_actionQueryProducts_triggered()
{
    p_mdl->setQuery(QString(
                     "SELECT ProductTypes.name, Products.name, Products.properties,"
                     " Products.price, Products.package, Products.supply, Products.count"
                     " FROM Products"
                     " INNER JOIN ProductTypes ON (ProductTypes.id = Products.type)"
                         ));

    if (p_mdl->lastError().isValid())
         qDebug() << p_mdl->lastError();

    p_mdl->setHeaderData(0, Qt::Horizontal, tr("Тип"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Наименование"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Свойства"));
    p_mdl->setHeaderData(3, Qt::Horizontal, tr("Цена"));
    p_mdl->setHeaderData(4, Qt::Horizontal, tr("Упаковка"));
    p_mdl->setHeaderData(5, Qt::Horizontal, tr("Поставка"));
    p_mdl->setHeaderData(6, Qt::Horizontal, tr("Количество"));
    p_mdl->sort(1,Qt::AscendingOrder);

    ui->tableView->setItemDelegateForColumn(3, pd);

    query_title = "Номенклатура предлагаемой на продажу продукции";
}

void MainWindow::on_actionQueryClients_triggered()
{
    DialogQueryClients dlg;
    int ret = dlg.exec();
    qDebug() << ret;
    int type = dlg.getType();
    p_mdl->setQuery(QString(
                    "SELECT Clients.name, Clients.phone, Clients.notes"
                    " FROM Clients"
                    " INNER JOIN SaleContracts ON (SaleContracts.client = Clients.id)"
                    " INNER JOIN ProductsSale ON (ProductsSale.contract = SaleContracts.id)"
                    " INNER JOIN Products ON (Products.id = ProductsSale.product)"
                    " INNER JOIN ProductTypes ON (ProductTypes.id = Products.type)"
                    " WHERE Products.type = %1").arg(type)
                );

    if (p_mdl->lastError().isValid())
         qDebug() << p_mdl->lastError();
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("Ф. И. О."));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Телефон"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Заметки"));
    p_mdl->sort(0,Qt::AscendingOrder);

    query_title = "Список клиентов по типу запрашиваемых товаров";
}
void MainWindow::on_actionQueryPotentialClients_triggered()
{
    p_mdl->setQuery(QString(
                    "SELECT Clients.name, Clients.phone, Clients.notes"
                    " FROM Clients"
                    " LEFT JOIN SaleContracts ON ( SaleContracts.client = Clients.id)"
                    " WHERE SaleContracts.client IS NULL"
                    )
                );

    if (p_mdl->lastError().isValid())
         qDebug() << p_mdl->lastError();
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("Ф. И. О."));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Телефон"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Заметки"));
    p_mdl->sort(0,Qt::AscendingOrder);

    query_title = "Список потенциальных клиентов";
}
void MainWindow::on_actionQuerySaleSupplyDone_triggered()
{
    p_mdl->setQuery(QString(
                    "SELECT supplycount, salecount, Products.name FROM"
                    " (SELECT product, count AS supplycount, NULL AS salecount"
                    " FROM ProductsSupply WHERE ProductsSupply.received IS 'true'"
                    " UNION ALL"
                    " SELECT product, NULL AS supplycount, count AS salecount"
                    " FROM ProductsSale WHERE ProductsSale.shipped IS 'true')"
                    " AS tblA"
                    " INNER JOIN Products ON (Products.id = tblA.product)"
                    )
                );

    if (p_mdl->lastError().isValid())
         qDebug() << p_mdl->lastError();
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("Поставлено"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Продано"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Наименование товара"));
    p_mdl->sort(2,Qt::AscendingOrder);

    query_title = "Отчет о поставленных и проданных товарах";
}
void MainWindow::on_actionQueryAllProducts_triggered()
{
    p_mdl->setQuery(QString(
                    "SELECT Products.name, Products.count FROM Products"
                    " WHERE Products.count IS NOT 0"
                    )
                );

    if (p_mdl->lastError().isValid())
         qDebug() << p_mdl->lastError();
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("Наименование"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Количество"));
    p_mdl->sort(0,Qt::AscendingOrder);

    query_title = "Отчет о товарах, находящихся на складе";
}
void MainWindow::on_actionQueryPaid_triggered()
{
    p_mdl->setQuery(QString(
                    "SELECT * FROM"
                    " (SELECT NULL AS supplyId, id AS saleId, date, total_price"
                    " FROM Checks WHERE Checks.paid IS 'true'"
                    " UNION ALL"
                    " SELECT id AS supplyId, NULL AS saleId, date, total_price"
                    " FROM Payments WHERE Payments.paid IS 'true')"
                    )
                );

    if (p_mdl->lastError().isValid())
         qDebug() << p_mdl->lastError();
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("№ платежа поставки"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("№ счета продажи"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Дата"));
    p_mdl->setHeaderData(3, Qt::Horizontal, tr("Сумма"));
    p_mdl->sort(2,Qt::AscendingOrder);

    ui->tableView->setItemDelegateForColumn(3, pd);

    query_title = "Отчет об оплате счетов и суммах оплаты";
}
void MainWindow::on_actionQueryUnpaid_triggered()
{
    p_mdl->setQuery(QString(
                    "SELECT * FROM"
                    " (SELECT NULL AS supplyId, id AS saleId, date, total_price"
                    " FROM Checks WHERE Checks.paid IS 'false'"
                    " UNION ALL"
                    " SELECT id AS supplyId, NULL AS saleId, date, total_price"
                    " FROM Payments WHERE Payments.paid IS 'false')"
                    )
                );

    if (p_mdl->lastError().isValid())
         qDebug() << p_mdl->lastError();
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("№ платежа поставки"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("№ счета продажи"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Дата"));
    p_mdl->setHeaderData(3, Qt::Horizontal, tr("Сумма"));
    p_mdl->sort(2,Qt::AscendingOrder);

    ui->tableView->setItemDelegateForColumn(3, pd);

    query_title = "Отчет о неоплаченных счетах";
}
void MainWindow::on_actionQuerySaleSupplyWait_triggered()
{
    p_mdl->setQuery(QString(
                    "SELECT supplycount, salecount, Products.name FROM"
                    " (SELECT product, count AS supplycount, NULL AS salecount"
                    " FROM ProductsSupply WHERE ProductsSupply.received IS 'false'"
                    " UNION ALL"
                    " SELECT product, NULL AS supplycount, count AS salecount"
                    " FROM ProductsSale WHERE ProductsSale.shipped IS 'false')"
                    " AS tblA"
                    " INNER JOIN Products ON (Products.id = tblA.product)"
                    )
                );

    if (p_mdl->lastError().isValid())
         qDebug() << p_mdl->lastError();
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("Ожидает поставки"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("Ожидает отгрузки"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Наименование товара"));
    p_mdl->sort(2,Qt::AscendingOrder);

    query_title = "Список заказываемых товаров";
}
void MainWindow::on_actionQueryContracts_triggered()
{
    p_mdl->setQuery(QString(
                    "SELECT supplyId,saleId, date, Vendors.name, Clients.name FROM"
                    " (SELECT NULL AS supplyId, id AS saleId, date, NULL AS vendor, client"
                    " FROM SaleContracts"
                    " UNION ALL"
                    " SELECT id AS supplyId, NULL AS saleId, date, vendor, NULL AS client"
                    " FROM SupplyContracts)"
                    " AS tblA"
                    " LEFT JOIN Clients ON (Clients.id = tblA.client)"
                    " LEFT JOIN Vendors ON (Vendors.id = tblA.vendor)"
                    )
                );

    if (p_mdl->lastError().isValid())
         qDebug() << p_mdl->lastError();
    p_mdl->setHeaderData(0, Qt::Horizontal, tr("№ договора поставки"));
    p_mdl->setHeaderData(1, Qt::Horizontal, tr("№ договора продажи"));
    p_mdl->setHeaderData(2, Qt::Horizontal, tr("Дата"));
    p_mdl->setHeaderData(3, Qt::Horizontal, tr("Производитель"));
    p_mdl->setHeaderData(4, Qt::Horizontal, tr("Клиент"));
    p_mdl->sort(2,Qt::AscendingOrder);

    ui->tableView->setItemDelegateForColumn(3,def_dlg);

    query_title = "Отчет о заключенных договорах на поставку и продажу товара";
}

void MainWindow::on_pushButtonUpdate_clicked()
{
    //выполним последний запрос
    p_mdl->setQuery(p_mdl->query().lastQuery());
}

void MainWindow::on_actionVendors_triggered()
{
    DialogTableDB dlg(this, DialogTableDB::Vendors);
    dlg.setModal(true);
    dlg.exec();
}
void MainWindow::on_actionClients_triggered()
{
    DialogTableDB dlg(this, DialogTableDB::Clients);
    dlg.setModal(true);
    dlg.exec();
}

void MainWindow::on_actionProducts_triggered()
{
    DialogTableDB dlg(this, DialogTableDB::Products);
    dlg.setModal(true);
    dlg.exec();
}
void MainWindow::on_actionProductTypes_triggered()
{
    DialogTableDB dlg(this, DialogTableDB::ProductTypes);
    dlg.setModal(true);
    dlg.exec();
}
void MainWindow::on_actionContractsSale_triggered()
{
    DialogTableDB dlg(this, DialogTableDB::SaleContracts);
    dlg.setModal(true);
    dlg.exec();
}
void MainWindow::on_actionChecks_triggered()
{
    DialogTableDB dlg(this, DialogTableDB::Checks);
    dlg.setModal(true);
    dlg.exec();
}
void MainWindow::on_actionProductsSale_triggered()
{
    DialogTableDB dlg(this, DialogTableDB::ProductsSale);
    dlg.setModal(true);
    dlg.exec();
}
void MainWindow::on_actionContractsSupply_triggered()
{
    DialogTableDB dlg(this, DialogTableDB::SupplyContracts);
    dlg.setModal(true);
    dlg.exec();
}
void MainWindow::on_actionPayments_triggered()
{
    DialogTableDB dlg(this, DialogTableDB::Payments);
    dlg.setModal(true);
    dlg.exec();
}
void MainWindow::on_actionProductsSupply_triggered()
{
    DialogTableDB dlg(this, DialogTableDB::ProductsSupply);
    dlg.setModal(true);
    dlg.exec();
}

void MainWindow::on_actionCreateDB_triggered()
{
    db_name = QFileDialog::getSaveFileName(this,
                           tr("Создать базу данных"), "./",
                           tr("Файлы баз данных sqlite (*.db)"));
    if(db_name.isEmpty())
        return;
    if(QFile::exists(db_name))
        QFile::remove(db_name);

    if( !CreateConnection() ){
        return;
    }

    QFile init_db(":/sql/init_db.sql");
    if (!init_db.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Не удалось открыть ресурс init_db.sql на чтение";
        return;
    }
    QSqlQuery query;
    QString sql_data = init_db.readAll();
    QString sql_parse;
    for(QString str : sql_data.split('\n')){
        QString sstr = str.simplified();
        if( !sstr.isEmpty() && !sstr.startsWith("--") ) {
            sql_parse.append(sstr + " ");
        }
    }
    for(QString str : sql_parse.split(';')){
        QString what = str.simplified();
        if (what.isEmpty())
            continue;
        if ( !query.exec(what) ){
            qDebug() << "Не удалось выполнить запрос из init_db.sql :";
            qDebug() << what;
        }
    }
    init_db.close();
    if(!CheckDatabase()){
        db.close();
        return;
    }
    ui->menuDB->setEnabled(true);
    ui->menuQuery->setEnabled(true);
}
void MainWindow::on_actionOpenDB_triggered()
{

    db_name = QFileDialog::getOpenFileName(this,
                           tr("Открыть базу данных"), "./",
                           tr("Файлы баз данных sqlite (*.db)"));
    if(db_name.isEmpty())
        return;

    if(db.isOpen()){
        db.commit();
        db.close();
    }
    ui->menuDB->setEnabled(false);
    ui->menuQuery->setEnabled(false);

    if(QFile::exists(db_name)){
        if( CreateConnection() && CheckDatabase() ) {
            ui->menuDB->setEnabled(true);
            ui->menuQuery->setEnabled(true);
            return;
        }
    } else {
        QMessageBox msgBox;
        msgBox.setText("Файл не найден!");
        msgBox.setInformativeText(db_name);
        msgBox.exec();
        return;
    }
    QMessageBox msgBox;
    msgBox.setText("Сбой подключения к базе данных!");
    msgBox.setInformativeText(db_name);
    msgBox.exec();
}

void MainWindow::on_pushButtonPrint_clicked()
{
    if(p_mdl->rowCount()<1){
        QMessageBox msgBox;
        msgBox.setText("Нет данных для печати.");
        msgBox.exec();
        return;
    }
    QTextDocument doc;  //Формируемый документ
    QTextCursor cur(&doc);  //Указатель на текущее место в документе
    QSqlRecord record;
    QTextTableFormat tableFormat;   //Настройки форматирования таблицы
    QTextCharFormat textFormat;

    //Задание формата отображения таблицы
    tableFormat.setHeaderRowCount(1);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorder(0.5);
    tableFormat.setBorderBrush(QBrush(Qt::gray,Qt::SolidPattern));
    tableFormat.setWidth(QTextLength(QTextLength::PercentageLength,100));

    textFormat.setFontWeight(QFont::Bold);
    cur.mergeCharFormat(textFormat);

    //Теперь, перемещая курсор по файлу начинается его заполнение данными
    cur.insertText(query_title);   //Добавление обычного текста

    textFormat.setFontWeight(QFont::Normal);
    //Добавление таблицы
    cur.insertTable(p_mdl->rowCount()+1,p_mdl->columnCount(),tableFormat); //Задаётся число строк, столбцов и формат
    for(int i=0;i<p_mdl->columnCount();i++)
    {
        cur.insertText(p_mdl->headerData(i,Qt::Horizontal).toString(),textFormat);
        cur.movePosition(QTextCursor::NextCell);
    }

    for(int i=0;i<p_mdl->rowCount();i++)
    {
        record=p_mdl->record(i); //Взятие из таблицы нужной записи
        for(int j=0;j<p_mdl->columnCount();j++){
            cur.insertText(record.value(j).toString());
            cur.movePosition(QTextCursor::NextCell);
        }
    }

    QPrinter printer(QPrinter::HighResolution);  //Создание принтера

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        doc.print(&printer); //Печать документа на выбранном принтере
    }
}
