Sample init scripts and service configuration for groincoind
==========================================================

Sample scripts and configuration files for systemd, Upstart and OpenRC
can be found in the contrib/init folder.

    contrib/init/groincoind.service:    systemd service unit configuration
    contrib/init/groincoind.openrc:     OpenRC compatible SysV style init script
    contrib/init/groincoind.openrcconf: OpenRC conf.d file
    contrib/init/groincoind.conf:       Upstart service configuration file
    contrib/init/groincoind.init:       CentOS compatible SysV style init script

Service User
---------------------------------

All three Linux startup configurations assume the existence of a "groincoin" user
and group.  They must be created before attempting to use these scripts.
The OS X configuration assumes groincoind will be set up for the current user.

Configuration
---------------------------------

At a bare minimum, groincoind requires that the rpcpassword setting be set
when running as a daemon.  If the configuration file does not exist or this
setting is not set, groincoind will shutdown promptly after startup.

This password does not have to be remembered or typed as it is mostly used
as a fixed token that groincoind and client programs read from the configuration
file, however it is recommended that a strong and secure password be used
as this password is security critical to securing the wallet should the
wallet be enabled.

If groincoind is run with the "-server" flag (set by default), and no rpcpassword is set,
it will use a special cookie file for authentication. The cookie is generated with random
content when the daemon starts, and deleted when it exits. Read access to this file
controls who can access it through RPC.

By default the cookie is stored in the data directory, but it's location can be overridden
with the option '-rpccookiefile'.

This allows for running groincoind without having to do any manual configuration.

`conf`, `pid`, and `wallet` accept relative paths which are interpreted as
relative to the data directory. `wallet` *only* supports relative paths.

For an example configuration file that describes the configuration settings,
see `contrib/debian/examples/groincoin.conf`.

Paths
---------------------------------

### Linux

All three configurations assume several paths that might need to be adjusted.

Binary:              `/usr/bin/groincoind`  
Configuration file:  `/etc/groincoin/groincoin.conf`  
Data directory:      `/var/lib/groincoind`  
PID file:            `/var/run/groincoind/groincoind.pid` (OpenRC and Upstart) or `/var/lib/groincoind/groincoind.pid` (systemd)  
Lock file:           `/var/lock/subsys/groincoind` (CentOS)  

The configuration file, PID directory (if applicable) and data directory
should all be owned by the groincoin user and group.  It is advised for security
reasons to make the configuration file and data directory only readable by the
groincoin user and group.  Access to groincoin-cli and other groincoind rpc clients
can then be controlled by group membership.

### Mac OS X

Binary:              `/usr/local/bin/groincoind`  
Configuration file:  `~/Library/Application Support/Groincoin/groincoin.conf`  
Data directory:      `~/Library/Application Support/Groincoin`  
Lock file:           `~/Library/Application Support/Groincoin/.lock`  

Installing Service Configuration
-----------------------------------

### systemd

Installing this .service file consists of just copying it to
/usr/lib/systemd/system directory, followed by the command
`systemctl daemon-reload` in order to update running systemd configuration.

To test, run `systemctl start groincoind` and to enable for system startup run
`systemctl enable groincoind`

### OpenRC

Rename groincoind.openrc to groincoind and drop it in /etc/init.d.  Double
check ownership and permissions and make it executable.  Test it with
`/etc/init.d/groincoind start` and configure it to run on startup with
`rc-update add groincoind`

### Upstart (for Debian/Ubuntu based distributions)

Drop groincoind.conf in /etc/init.  Test by running `service groincoind start`
it will automatically start on reboot.

NOTE: This script is incompatible with CentOS 5 and Amazon Linux 2014 as they
use old versions of Upstart and do not supply the start-stop-daemon utility.

### CentOS

Copy groincoind.init to /etc/init.d/groincoind. Test by running `service groincoind start`.

Using this script, you can adjust the path and flags to the groincoind program by
setting the GROINCOIND and FLAGS environment variables in the file
/etc/sysconfig/groincoind. You can also use the DAEMONOPTS environment variable here.

### Mac OS X

Copy org.groincoin.groincoind.plist into ~/Library/LaunchAgents. Load the launch agent by
running `launchctl load ~/Library/LaunchAgents/org.groincoin.groincoind.plist`.

This Launch Agent will cause groincoind to start whenever the user logs in.

NOTE: This approach is intended for those wanting to run groincoind as the current user.
You will need to modify org.groincoin.groincoind.plist if you intend to use it as a
Launch Daemon with a dedicated groincoin user.

Auto-respawn
-----------------------------------

Auto respawning is currently only configured for Upstart and systemd.
Reasonable defaults have been chosen but YMMV.
