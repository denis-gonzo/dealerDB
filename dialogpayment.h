#pragma once

#include <QDialog>

class QSqlTableModel;

namespace Ui {
class DialogPayment;
}

class DialogPayment : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPayment(QWidget *parent = nullptr, int cur_id = -1);
    ~DialogPayment();
private slots:
    void finishOnAccept();
private:
    Ui::DialogPayment *ui;
    QSqlTableModel *model;
    QSqlTableModel *contracts_model;
    int current_id;
};

