# Leka filesystem

LekaOS' filesystem is inspired by the unix [filesystem](https://en.wikipedia.org/wiki/Unix_filesystem).

## Directory layout

`/fs` is the root of the filesystem and directory layout. Everything else is located under `/fs`

|Directory|Description|
|---|---|
| `/etc` | "Edit To Configure" - Contains system-wide configuration files, e.g. battery charging hysteresis, number of reboots before factory rollback, screen saver timeout, etc. |
| `/home/{img,vid,wav}` | Keeps all user facing assets such as images, sounds, animations, videos, etc. |
| `/sys` | Contains information related to hardware and the operating system, such as mcuboot version, os version, hardware version, etc. |
| `/sys/factory` | Factory OS & settings. Read only. |
| `/usr` | "User System Resources"|
| `/usr/share` | Keeps shared resources between OS and bootloader, such as number of reboots |
| `/usr/certs` | Keeps all certificates |
| `/usr/os` | Keeps new os firmware updates |
| `/var` | "Variable" - A place for files that might change frequently |
| `/var/log` | Contains system log files |
| `/var/tmp` | A place for temporary files which should be preserved between system reboots, e.g. reboot timestamp, etc. |​

## Differences between `/usr` & `/home`

- `/usr` is *system/robot facing*
    - it contains everything the robot needs to work properly
    - things inside `/usr` are *understood* by the robot
    - modifications to files inside `/usr` will directly impact how the robot works

- `/home` is *external user facing*
    - it contains everything presented to the external user of the robot (images, sounds, videos, etc.)
    - things inside `/home` are **not** *understood* by the robot
    - modifications to files inside `/home` will impact what the external user sees, not how the robot works
