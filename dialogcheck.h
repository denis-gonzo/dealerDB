#pragma once

#include <QDialog>

class QSqlTableModel;

namespace Ui {
class DialogCheck;
}

class DialogCheck : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCheck(QWidget *parent = nullptr, int cur_id = -1);
    ~DialogCheck();
private slots:
    void finishOnAccept();
private:
    Ui::DialogCheck *ui;
    QSqlTableModel *model;
    QSqlTableModel *contracts_model;
    int current_id;
};

