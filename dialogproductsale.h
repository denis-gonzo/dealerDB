#pragma once

#include <QDialog>

class QSqlTableModel;

namespace Ui {
class DialogProductSale;
}

class DialogProductSale : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProductSale(QWidget *parent = nullptr, int cur_id = -1);
    ~DialogProductSale();
private slots:
    void finishOnAccept();
private:
    Ui::DialogProductSale *ui;
    QSqlTableModel *model;
    QSqlTableModel *products_model;
    QSqlTableModel *contracts_model;
    int current_id;
};

