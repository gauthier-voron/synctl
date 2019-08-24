Synctl (beta 0.1.0)
===================

Synctl is a backup and synchronization system.
Typical use cases for Synctl are:

  * Backup your system every hour in an external hard drive.

  * Backup your system every hour through ssh to a remote server.

  * Synchronize the files of two or more computers through a remote server.

Synctl is designed to be fast and lightweight. It can run on low consumption
servers like a Raspberry Pi and over a slow internet connection.


Beta disclaimer
---------------

Synctl is currently in beta version.
This means it is sufficiently mature to be tested on real world configuration.
However, it is not considered stable which means that it can, at any moment,
crash, corrupt your backups or even corrupt your system.
Be especially careful if you run it as root.
It is also strongly advised to *backup your system with another tool* before to
execute this beta version.


Build instructions
------------------

To build synctl, you need the following dependencies installed on your system:

  * gcc (version 7 or above)

  * libssl

  * libcrypto

  * libpcre2

  * make

To run synctl automatic tests, you also need the following dependencies
installed on your system:

  * perl

Then run the command:

```
make all
```

Compilation might take a few moments.
If your system supports it, consider enabling parallel compilation.


Install instructions
--------------------

Before installation, we *strongly* suggest to perform automatic tests.
To this end, run the command:

```
make test
```

Testing might take a few moments.
If the automatic test report indicate failed tests, do not try to use Synctl
and report the issue.

Some tests require additional privileges to run.
If you want to execute them, please consider using a sandbox environment like
Docker or VirtualBox.

If all the tests are successful, install Synctl with the command:

```
make install
```

You need to run this command with root privilege.


Execute instructions
--------------------

To get help about how to use Synctl, use the command:

```
synctl --help
```

You can also read the tutorial by running:

```
synctl help --tutorial
```


Author
------

To report an issue, please got to
<https://github.com/gauthier-voron/synctl/issues>.

To contact the author, send an email to <gauthier.voron@mnesic.fr>
