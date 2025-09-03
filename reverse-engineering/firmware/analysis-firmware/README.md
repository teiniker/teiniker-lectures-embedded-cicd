# Firmware Analysis

The aim of this introduction is to show how firmware can be analyzed.

The firmware to be examined can either be read from devices or downloaded 
from the Internet.
We are using explicit **training software** (IoTGoat, Damn Vulnerable Router Firmware (DVRF)) 
that was implemented for training purposes.

The tools we need for this static analysis are installed in **Kali Linux**.

## Static Analysis 

### IoT Goat 

The IoTGoat Project is a deliberately insecure firmware based on OpenWrt and maintained by OWASP as a platform 
to educate software developers and security professionals with testing commonly found vulnerabilities in IoT devices. 
The vulnerability challenges are based on the OWASP IoT Top 10 noted below, as well as "easter eggs" from project contributors.

We download the [firmware](https://github.com/OWASP/IoTGoat/releases) from GitHub.

[IoTGoat challenges](https://github.com/OWASP/IoTGoat/wiki/IoTGoat-challenges)
```
Download: IoTGoat-x86.img.gz

$ gunzip IoTGoat-x86.img.gz
```

### binwalk

Binwalk is a fast, easy to use tool for analyzing, reverse engineering, 
and extracting firmware images.

```
$ binwalk -e IoTGoat-x86.img 
DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
262144        0x40000         Linux EXT filesystem, blocks count: 4096, image size: 4194304, rev 2.0, ext2 filesystem data, UUID=57f8f4bc-abf4-655f-bf67-946fc0f9c0f9
5325930       0x51446A        xz compressed data
17301504      0x1080000       Squashfs filesystem, little endian, version 4.0, compression:xz, size: 3481630 bytes, 1352 inodes, blocksize: 262144 bytes, created: 2019-01-30 12:21:02

$ tree _IoTGoat-x86.img.extracted
```
Here we see, that the image contains a **Linux kernel** and a **Squashfs filesystem**.
It is not uncommon that IoT Devices run a Linux system because the need a WiFi network stack.

Within the extracted Linux filesystem, we can look for interesting files...


### firmwalker

A simple bash script for searching the extracted or mounted firmware file system is called 
[firmwalker](https://github.com/craigz28/firmwalker).
```
$ git clone https://github.com/craigz28/firmwalker.git
$ cd firmwalker

$ ./firmwalker/firmwalker.sh _IoTGoat-x86.img.extracted
***Firmware Directory***
../_IoTGoat-x86.img.extracted
***Search for password files***
##################################### passwd
d/squashfs-root/bin/passwd
d/squashfs-root/etc/passwd

##################################### shadow
d/squashfs-root/etc/shadow
...
```

An interesting resource is the `/etc/shadow` file, in which user names 
and passwords (encrypted) are stored under Linux.

_Example_: Read `/etc/shadow` file
```
$ less _IoTGoat-x86.img.extracted/squashfs-root/etc/shadow
root:$1$Jl7H1VOG$Wgw2F/C.nLNTC.4pwDa4H1:18145:0:99999:7:::
daemon:*:0:0:99999:7:::
ftp:*:0:0:99999:7:::
network:*:0:0:99999:7:::
nobody:*:0:0:99999:7:::
dnsmasq:x:0:0:99999:7:::
dnsmasq:x:0:0:99999:7:::
iotgoatuser:$1$79bz0K8z$Ii6Q/if83F1QodGmkb4Ah.:18145:0:99999:7:::
```


### Important Files in the Linux Filesystems 

Here’s a short list of key configuration files and directories in Linux that attackers love to get their hands on:

* There are lots of places in the Linux startup process where we can insert malicious commands 
	or scripts such as in in the boot loader (e.g. GRUB or LILO), Kernel parameters in `/proc/cmdline`, daemons and services in `/etc/system.d`, run commands in `/etc/rc.*` and 
    `/etc/init.*`.
* We can also set up scripts to run as cron jobs. But there are other crafty ways to cause 
    scripts to run with whenever a shell starts. For instance, with Bash, you need to watch 
    `/etc/profile`, `~/.bash_profile`, `~/.bash_login`, `~/.profile`, `/home/user/.bashrc`, 
    `/etc/bash.bashrc`, `/etc/profile.d/`.
* We can override DNS and cause your system to communicate with imposter systems by messing 
    with files like `/etc/hosts` and `/etc/resolv.conf`.
* Changes to `/etc/passwd`, `/etc/shadow`, `/etc/group`, `/etc/gshadow` are all super 
    important to monitor because this is where user accounts, groups and password hashes 
    are stored. 
	Related to that are files under `/etc/pam.d` where password and lockout policies are 
    stored and where sophisticated attackers can install bogus pluggable authentication 
    modules that steal passwords.


### Database Files 

Another important source of data are **SQLite database files**. 
They often contain large amounts of data.

_Example_: Read SQLite database file
* Start the **DB Browser for SQLite**
* Load the following file: `squashfs-root/usr/lib/lua/luci/controller/iotgoat/sensordata.db`
* Review the database table and its data.

* Alternative, we can use the **sqlite3 command line tool**:
    ```
    $ sqlite3 sensordata.db 
    SQLite version 3.42.0 2023-05-16 12:36:15
    Enter ".help" for usage hints.
    sqlite> .dump
    PRAGMA foreign_keys=OFF;
    BEGIN TRANSACTION;
    CREATE TABLE sensors(id INTEGER PRIMARY KEY AUTOINCREMENT, temperature NUMERIC, humidity NUMERIC, currentdate DATE, currentime TIME, name TEXT, email TEXT, birthdate NUMERIC);
    INSERT INTO sensors VALUES(1,22.399999999999998579,68,'2020-03-24','18:56:33','johnsmith','johnsmith@gmail.com',1311977);
    INSERT INTO sensors VALUES(2,29.699999999999999289,98,'2020-03-24','18:56:43','jillsmith','jillsmith@gmail.com',4141979);
    INSERT INTO sensors VALUES(3,31.199999999999999289,28,'2020-03-24','18:57:05','walter','waltergary@yopmail.com',32821969);
    INSERT INTO sensors VALUES(4,16.899999999999998578,38,'2020-03-24','18:57:20','WilliamRonald','billronald@yopmail.com',11141989);
    INSERT INTO sensors VALUES(5,35,78,'2020-03-24','18:58:04','Test','TstUser@aol.com',12121990);
    INSERT INTO sensors VALUES(6,35,88,'2020-03-24','18:58:18','Sgt','sgtmajor@us.gov',10171956);
    DELETE FROM sqlite_sequence;
    INSERT INTO sqlite_sequence VALUES('sensors',6);
    COMMIT;
    sqlite> .quit
    ```

Finally, we can **analyze binaries using Ghidra** to find hard coded secrets and 
interesting vulnerabilities.


## References
* [OWASP Firmware Security Testing Methodology](https://scriptingxss.gitbook.io/firmware-security-testing-methodology/)

* [Reverse Engineering the TP-Link HS110](https://www.softscheck.com/en/reverse-engineering-tp-link-hs110/)
* [YouTube: Simple Firmware Reverse Engineering](https://youtu.be/oqk3cU7ekag)
* [YouTube: IoT Firmware Analysis](https://youtu.be/bwZlwIOMkJE)

* Aditya Gupta. **The IoT Hacker's Handbook**. Apress, 2019
	* Chapter 7: Firmware Reverse Engineering and Exploitation  

* **Reverse Engineering Tools**
    * [Binwalk](https://github.com/ReFirmLabs/binwalk)
    * [Firmwalker](https://github.com/craigz28/firmwalker)

* **Training Firmware**
    * [IoTGoat Project](https://github.com/OWASP/IoTGoat)
    * [Damn Vulnerable Router Firmware (DVRF) v0.3](https://github.com/praetorian-inc/DVRF/tree/master/Firmware)

* [The Most Important Linux Files to Protect](https://www.beyondtrust.com/blog/entry/important-linux-files-protect)

*Egon Teiniker, 2023, GPL v3.0*
