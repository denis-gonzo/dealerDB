#pragma once

#include <QDialog>

class QSqlTableModel;

namespace Ui {
class DialogProduct;
}

class DialogProduct : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProduct(QWidget *parent = nullptr, int cur_id = -1);
    ~DialogProduct();

private slots:
    void finishOnAccept();


private:
    Ui::DialogProduct *ui;
    QSqlTableModel *model;
    QSqlTableModel *types_model;
    int current_id;
};

