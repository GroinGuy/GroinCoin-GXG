#!/bin/bash

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

GROINCOIND=${GROINCOIND:-$SRCDIR/groincoind}
GROINCOINCLI=${GROINCOINCLI:-$SRCDIR/groincoin-cli}
GROINCOINTX=${GROINCOINTX:-$SRCDIR/groincoin-tx}
GROINCOINQT=${GROINCOINQT:-$SRCDIR/qt/groincoin-qt}

[ ! -x $GROINCOIND ] && echo "$GROINCOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
GXGVER=($($GROINCOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for groincoind if --version-string is not set,
# but has different outcomes for groincoin-qt and groincoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$GROINCOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $GROINCOIND $GROINCOINCLI $GROINCOINTX $GROINCOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${GXGVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${GXGVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
