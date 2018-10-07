#ifndef GROINCOIN_QT_TEST_WALLETTESTS_H
#define GROINCOIN_QT_TEST_WALLETTESTS_H
// Copyright (c) 2014-2018 The Groincoin Core developers

#include <QObject>
#include <QTest>

class WalletTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void walletTests();
};

#endif // GROINCOIN_QT_TEST_WALLETTESTS_H
