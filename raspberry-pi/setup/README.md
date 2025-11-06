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


## Shell Configurations

* Change super user password:
  * username: root
  * password: root66
  
```bash
$ sudo su -
passwd
```

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

## Delete bash history

```bash
$ rm ~/.bash_history
```

## Win32 Disk Imager – Copy SD Card Images

### 1. Create an Image (Copy SD Card → PC)

* Insert the SD card into the PC’s card reader.
* Open **Win32 Disk Imager** (as Administrator).
* **Image File** field:
  - Choose a location and name for the backup image (e.g., `C:\<USER>\rpi5_student_image_2025.img`).
* **Device** dropdown:
  - Select your SD card path (e.g., `D:`).
* Click **Read**.
  - The SD card is read and saved as an image file (`.img`) at the chosen location on your PC.
* Wait for the process to complete (takes about 30 minutes).

### 2. Write an Image (Copy Image → SD Card)

* Insert the target SD card into your PC.
* **Image File** field:
  - Browse and select the `.img` file you want to write (e.g., `C:\<USER>\rpi5_student_image_2025.img`).
* **Device** dropdown:
  - Select the drive letter of your SD card (e.g., `D:`).
* Click **Write** to start writing the image.
* Wait until the process completes (takes about 15–20 minutes).
  
## Change hostname 

Replace XX with the kit number:

```bash
$ sudo hostnamectl set-hostname ece-rpi5-kitXX
```

Update the `/etc/hosts` to the correct kit number: 
```bash
$ sudo vim /etc/hosts
```

Reboot the Raspberry: 
```bash
$ sudo reboot
```

*Egon Teiniker, 2024-2025, GPL v3.0*
