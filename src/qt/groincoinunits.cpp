// Copyright (c) 2011-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// Copyright (c) 2014-2018 The Groincoin Core developers
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/groincoinunits.h>

#include <primitives/transaction.h>

#include <QStringList>

GroincoinUnits::GroincoinUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<GroincoinUnits::Unit> GroincoinUnits::availableUnits()
{
    QList<GroincoinUnits::Unit> unitlist;
    unitlist.append(GXG);
    unitlist.append(mGXG);
    unitlist.append(uGXG);
    return unitlist;
}

bool GroincoinUnits::valid(int unit)
{
    switch(unit)
    {
    case GXG:
    case mGXG:
    case uGXG:
        return true;
    default:
        return false;
    }
}

QString GroincoinUnits::longName(int unit)
{
    switch(unit)
    {
    case GXG: return QString("GXG");
    case mGXG: return QString("mGXG");
    case uGXG: return QString::fromUtf8("µGXG (bits)");
    default: return QString("???");
    }
}

QString GroincoinUnits::shortName(int unit)
{
    switch(unit)
    {
    case uGXG: return QString::fromUtf8("bits");
    default:   return longName(unit);
    }
}

QString GroincoinUnits::description(int unit)
{
    switch(unit)
    {
    case GXG: return QString("Groincoins");
    case mGXG: return QString("Milli-Groincoins (1 / 1" THIN_SP_UTF8 "000)");
    case uGXG: return QString("Micro-Groincoins (bits) (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
    default: return QString("???");
    }
}

qint64 GroincoinUnits::factor(int unit)
{
    switch(unit)
    {
    case GXG:  return 100000000;
    case mGXG: return 100000;
    case uGXG: return 100;
    default:   return 100000000;
    }
}

int GroincoinUnits::decimals(int unit)
{
    switch(unit)
    {
    case GXG: return 8;
    case mGXG: return 5;
    case uGXG: return 2;
    default: return 0;
    }
}

QString GroincoinUnits::format(int unit, const CAmount& nIn, bool fPlus, SeparatorStyle separators)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 n = (qint64)nIn;
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    qint64 remainder = n_abs % coin;
    QString quotient_str = QString::number(quotient);
    QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');

    // Use SI-style thin space separators as these are locale independent and can't be
    // confused with the decimal marker.
    QChar thin_sp(THIN_SP_CP);
    int q_size = quotient_str.size();
    if (separators == separatorAlways || (separators == separatorStandard && q_size > 4))
        for (int i = 3; i < q_size; i += 3)
            quotient_str.insert(q_size - i, thin_sp);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');
    return quotient_str + QString(".") + remainder_str;
}


// NOTE: Using formatWithUnit in an HTML context risks wrapping
// quantities at the thousands separator. More subtly, it also results
// in a standard space rather than a thin space, due to a bug in Qt's
// XML whitespace canonicalisation
//
// Please take care to use formatHtmlWithUnit instead, when
// appropriate.

QString GroincoinUnits::formatWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    return format(unit, amount, plussign, separators) + QString(" ") + shortName(unit);
}

QString GroincoinUnits::formatHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(formatWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}


bool GroincoinUnits::parse(int unit, const QString &value, CAmount *val_out)
{
    if(!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);

    // Ignore spaces and thin spaces when parsing
    QStringList parts = removeSpaces(value).split(".");

    if(parts.size() > 2)
    {
        return false; // More than one dot
    }
    QString whole = parts[0];
    QString decimals;

    if(parts.size() > 1)
    {
        decimals = parts[1];
    }
    if(decimals.size() > num_decimals)
    {
        return false; // Exceeds max precision
    }
    bool ok = false;
    QString str = whole + decimals.leftJustified(num_decimals, '0');

    if(str.size() > 18)
    {
        return false; // Longer numbers will exceed 63 bits
    }
    CAmount retvalue(str.toLongLong(&ok));
    if(val_out)
    {
        *val_out = retvalue;
    }
    return ok;
}

QString GroincoinUnits::getAmountColumnTitle(int unit)
{
    QString amountTitle = QObject::tr("Amount");
    if (GroincoinUnits::valid(unit))
    {
        amountTitle += " ("+GroincoinUnits::shortName(unit) + ")";
    }
    return amountTitle;
}

int GroincoinUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant GroincoinUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(longName(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}

CAmount GroincoinUnits::maxMoney()
{
    return MAX_MONEY;
}
