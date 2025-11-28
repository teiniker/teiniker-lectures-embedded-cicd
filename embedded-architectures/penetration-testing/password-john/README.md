# Password Cracking with John the Ripper

John the Ripper is an Open Source password security auditing and password recovery tool available for many 
operating systems.

## Setup
```
$ sudo apt install john
```

_Tip_: Remove the `.john` directory before you run these examples.
```
$ sudo rm -r /root/.john
```

## Cracking Linux Passwords
```
$ cat shadow-debian13.txt
student:$y$j9T$dCDbCiiBA.WjzbkPqEP4t0$0WQv9GiN57wAKXuWyrvGTD.okhL2m/w1CjZ7Gob.XxB:20326:0:99999:7:::
root:$y$j9T$6F5vcCT6edWN2NSRYO4bM/$dBgAtXLDeQPYRFZjl46JevPpjaGrs4fClTW4KybUER.:20326:0:99999:7:::

$ sudo john --format=crypt shadow-debian13.txt

Loaded 2 password hashes with 2 different salts (crypt, generic crypt(3) [?/64])
Will run 4 OpenMP threads
Press 'q' or Ctrl-C to abort, almost any other key for status
student          (student)
root66           (root)
2g 0:00:00:08 100% 1/3 0.2475g/s 249.3p/s 249.5c/s 249.5C/s -root-..9999969
Use the "--show" option to display all of the cracked passwords reliably
Session completed
```


## References
* [John The Ripper Hash Formats](https://pentestmonkey.net/cheat-sheet/john-the-ripper-hash-formats)

* [Youtube: Password Cracking With John The Ripper](https://youtu.be/XjVYl1Ts6XI)

* [John the Ripper Password Cracker](https://www.openwall.com/john/) 
	
* [10k Most Common Passwords](https://github.com/danielmiessler/SecLists/blob/master/Passwords/Common-Credentials/10k-most-common.txt)	

* [crunch - Wordlist Generator](https://sourceforge.net/p/crunch-wordlist/code/ci/master/tree/)

  
*Egon Teiniker, 2024-2025, GPL v3.0*	