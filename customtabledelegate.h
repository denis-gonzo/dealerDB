#pragma once

#include <QStyledItemDelegate>

class PriceDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    PriceDelegate(QObject *parent = 0) : QStyledItemDelegate(parent) {}

    QString displayText(const QVariant &value, const QLocale &locale) const
    {
        Q_UNUSED(locale)
        QString str = QString::number(value.toDouble(), 'f', 2);
        return str;
    }
};
