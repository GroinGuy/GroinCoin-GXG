// Copyright (c) 2014-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// Copyright (c) 2014-2018 The Groincoin Core developers
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GROINCOIN_ZMQ_ZMQCONFIG_H
#define GROINCOIN_ZMQ_ZMQCONFIG_H

#if defined(HAVE_CONFIG_H)
#include <config/groincoin-config.h>
#endif

#include <stdarg.h>
#include <string>

#if ENABLE_ZMQ
#include <zmq.h>
#endif

#include <primitives/block.h>
#include <primitives/transaction.h>

void zmqError(const char *str);

#endif // GROINCOIN_ZMQ_ZMQCONFIG_H
