# SSH-Connection 

This guide shows how to connect to your Raspberry Pi over SSH and configure 
an SSH key pair for secure, passwordless login.

**Note:** Ensure your Raspberry Pi is connected to the WLAN: `FHJOANNEUM2work`.

## Connect to the Raspberry Pi: 

```bash
$ ssh student@xxx.xxx.xxx.xxx
```

**Note:** Replace `xxx.xxx.xxx.xxx` with the actual IP address of your Raspberry Pi.

Change your Passwords (for root and student):

```bash
$ passwd
```

## SSH Connection from the **Linux VM**

Generate an SSH Key, open a terminal and create a key:

```bash 
$ ssh-keygen -t ed25519
```

Use the `ssh-copy-id` command to copy your public key to the Raspberry Pi:

```bash 
$ ssh-copy-id -i ~/.ssh/id_ed25519.pub student@xxx.xxx.xxx.xxx
```

This will copy the public key to your home directory on the Raspberry Pi 5 into the following directory: 

```
$ ~/.ssh/authorized_keys
```

## Test the Connection without using a password: 

```bash
$ ssh student@xxx.xxx.xxx.xxx
```

## References

* [Raspberry Pi: access a remote terminal with SSH](https://www.raspberrypi.com/documentation/computers/remote-access.html#ssh)
* [Linux man page: passwd](https://linux.die.net/man/1/passwd)	

*Nicoletta Kähling, 2025, GPL v3.0*
