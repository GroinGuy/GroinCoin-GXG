
Debian
====================
This directory contains files used to package groincoind/groincoin-qt
for Debian-based Linux systems. If you compile groincoind/groincoin-qt yourself, there are some useful files here.

## groincoin: URI support ##


groincoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install groincoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your groincoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/groincoin128.png` to `/usr/share/pixmaps`

groincoin-qt.protocol (KDE)

