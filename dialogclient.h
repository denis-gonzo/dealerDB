#pragma once

#include <QDialog>

class QSqlTableModel;

namespace Ui {
class DialogClient;
}

class DialogClient : public QDialog
{
    Q_OBJECT

public:
    explicit DialogClient(QWidget *parent = nullptr, int cur_id = -1);
    ~DialogClient();
private slots:
    void finishOnAccept();
private:
    Ui::DialogClient *ui;
    QSqlTableModel *model;
    int current_id;
};

