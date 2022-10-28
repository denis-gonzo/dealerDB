#pragma once

#include <QDialog>

class QSqlTableModel;

namespace Ui {
class DialogProductSupply;
}

class DialogProductSupply : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProductSupply(QWidget *parent = nullptr, int cur_id = -1);
    ~DialogProductSupply();
private slots:
    void finishOnAccept();
private:
    Ui::DialogProductSupply *ui;
    QSqlTableModel *model;
    QSqlTableModel *products_model;
    QSqlTableModel *contracts_model;
    int current_id;
};

