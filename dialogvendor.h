#pragma once

#include <QDialog>

class QSqlTableModel;

namespace Ui {
class DialogVendor;
}

class DialogVendor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogVendor(QWidget *parent = nullptr, int cur_id = -1);
    ~DialogVendor();
private slots:
    void finishOnAccept();
private:
    Ui::DialogVendor *ui;
    QSqlTableModel *model;
    int current_id;
};

