#pragma once

#include <QDialog>

class QSqlTableModel;

namespace Ui {
class DialogQueryClients;
}

class DialogQueryClients : public QDialog
{
    Q_OBJECT

public:
    explicit DialogQueryClients(QWidget *parent = nullptr);
    ~DialogQueryClients();

    int getType();

private:
    Ui::DialogQueryClients *ui;
    int m_type;
    QSqlTableModel* mdl;
};

