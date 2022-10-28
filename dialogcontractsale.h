#pragma once

#include <QDialog>

class QSqlTableModel;

namespace Ui {
class DialogContractSale;
}

class DialogContractSale : public QDialog
{
    Q_OBJECT

public:
    explicit DialogContractSale(QWidget *parent = nullptr, int cur_id = -1);
    ~DialogContractSale();
private slots:
    void finishOnAccept();
private:
    Ui::DialogContractSale *ui;
    QSqlTableModel *model;
    QSqlTableModel *clients_model;
    int current_id;
};

