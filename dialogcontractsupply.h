#pragma once

#include <QDialog>

class QSqlTableModel;

namespace Ui {
class DialogContractSupply;
}

class DialogContractSupply : public QDialog
{
    Q_OBJECT

public:
    explicit DialogContractSupply(QWidget *parent = nullptr, int cur_id = -1);
    ~DialogContractSupply();
private slots:
    void finishOnAccept();
private:
    Ui::DialogContractSupply *ui;
    QSqlTableModel *model;
    QSqlTableModel *vendors_model;
    int current_id;
};

