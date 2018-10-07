#include "univalue.h"

// Copyright (c) 2014-2018 The Groincoin Core developers
int main (int argc, char *argv[])
{
    char buf[] = "___[1,2,3]___";
    UniValue val;
    return val.read(buf + 3, 7) ? 0 : 1;
}
