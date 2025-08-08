# Description

Edito is a high-performance IDE integrated into the Linux terminal.

It is entirely coded in C and ncurses.

![alt text](https://github.com/Doriiaan/Edito/blob/main/doc/Edito.png)

The features are as follows:
- Open a folder as a project
- Open folders and files within the project.
- Change modes (directory, bar, file)
- Edit and save files

# Prerequisites

- ncurses version 6
- ncurses-devel version 6
- GCC

# Installing Required Libraries

## On Debian/Ubuntu

```sh
sudo apt-get update
sudo apt-get install libncurses6 libncurses-dev

cd Edito/
make
cd ./build
./edito [directory]
```

## On Redhat/Centos

```sh
sudo dnf install ncurses ncurses-devel

cd Edito/
make
cd ./build
./edito [directory]
```

# Licence

This project is licensed under the terms of the GPL 3.0 license.

# Contributors

If anyone is interested in taking over the project, please email me at alary.dorian81@gmail.com. This project is not my priority. But it's still functional and useful.
