#pragma once

#include <QDialog>

class QSqlTableModel;

namespace Ui {
class DialogProductType;
}

class DialogProductType : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProductType(QWidget *parent = nullptr, int cur_id = -1);
    ~DialogProductType();
private slots:
    void finishOnAccept();
private:
    Ui::DialogProductType *ui;
    QSqlTableModel *model;
    int current_id;
};

