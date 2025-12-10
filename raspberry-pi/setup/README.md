# Raspberry Pi Setup 

## Raspberry Pi Imager

Raspberry Pi OS is a free, **Debian-based operating system** optimised for 
the Raspberry Pi hardware.

* [Dokumentation](https://www.raspberrypi.com/documentation/computers/os.html#introduction)
* [Software](https://www.raspberrypi.com/software/)

* [YouTube: Raspberry Pi 5 Setup: Getting Started Guide (Step By Step)](https://youtu.be/ZH6vfvRstfM?si=xTgGDdv1LJ3DFrtX)

We use the **Raspberry Pi Imager** to install Raspberry Pi OS on an SD card:
* Raspberry Pi Model: RASPBERRY PI 5 
* Operating System: Raspberry Pi OS (64-bit)
* SD-Card: drive 

### OS customization:

* Set username and password
  * username: student
  * password: student

* Configure wireless LAN:
  * SSID: FHJOANNEUM4iot
  * Password: <IOT Password>

* Set locale settings:
  * Time zone: Europe/Vienna
  * Keyboard layout: de

* Change super user password:
  * username: root
  * password: root66
  
```bash
$ sudo su -
passwd
```

## Shell Configurations

* Terminal Settings
	* Background: FFFFBB
	* Text Color: Black

* Keyboard Layout: Preferences / Mouse and Keyboard Settings / Keyboard / Keyboard Layout 


```Bash
$ sudo apt install vim

$ vim .bashrc 
	uncomment the following lines 
	alias ll='ls -l'
	alias la='ls -A'
	alias vi=vim
```


## Manage Software Packages

**Advanced Package Tool (APT)** is the recommended way to install, update, and remove 
software in Raspberry Pi OS.

`apt` stores a list of software sources in a file at `/etc/apt/sources.list`

```Bash
$ sudo apt update
$ sudo apt full-upgrade
$ sudo reboot
```

Unlike Debian, Raspberry Pi OS is under continual development. As a result, package 
dependencies sometimes change, so you should always use **full-upgrade** instead of the 
standard upgrade.

Run these commands regularly to keep your software up-to-date.


## Editors

```Bash
$ sudo apt install vim

$ vim .vimrc
	set number          " show line numbers
	syntax on           " syntax highlighting
	set tabstop=4       " The width of a TAB is set to 4.
	set shiftwidth=4    " Indents will have a width of 4
	set softtabstop=4   " Sets the number of columns for a TAB
	set expandtab       " Expand TABs to spaces			
```

```Bash
$ sudo apt install code    
```


## Programming

### C/C++ Programming 

Already installed: git, gcc, g++, gdb, make  

```Bash
$ sudo apt install clang
```

```Bash
$ sudo apt install valgrind
```

```Bash
$ sudo apt install ltrace
```

```Bash
$ sudo apt install strace
```

```Bash
$ sudo apt install cmake
```

```Bash
$ sudo apt install musl 
```

```Bash 
$ cd Downloads/
$ git clone https://github.com/google/googletest
$ cd googletest/
$ mkdir build
$ cd build/
$ cmake ..
$ make
$ sudo make install 

/usr/local/lib/libgmock.a
/usr/local/include/gtest/gtest.h
```

### Python Programming

Raspberry Pi OS comes with **Python 3 pre-installed**. 

```Bash
$ python --version
Python 3.11.2

$ pip --version
pip 23.0.1 from /usr/lib/python3/dist-packages/pip (python 3.11)
```

```Bash
$ pip install cryptography
```

```Bash
$ vim .bashrc 
    export PYTHON_LOCAL=/home/student/.local
    add $PYTHON_LOCAL/bin to the PATH environemnt variable 
```

### Programming Java 

* [Raspberry Pi: Installing Java](https://youtu.be/jhPnCCwhnKo?si=WqM9V0R6S8ELM9Nb)

```Bash
$ sudo apt install openjdk-21-jdk -y
$ sudo update-alternatives --config java 
```


### Embedded Development 

```Bash
$ sudo apt install python3-tk thonny
```

```Bash
 $ pip install --break-system-packages -U platformio
```


## Data Processing 

```Bash
$ sudo apt install sqlite3
$ sudo apt install sqlitebrowser
$ sudo apt install libsqlite3-dev
```


## Documentation 

```Bash
$ sudo apt install doxygen
```

```Bash
$ sudo apt install graphviz
```

```Bash
$ sudo apt install xpdf
```


*Egon Teiniker, 2024-2025, GPL v3.0*
