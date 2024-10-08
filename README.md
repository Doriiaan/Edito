# Description

Edito is a useful text editor for opening a directory and the directories under it. It lets you edit several files at once.

![alt text](https://github.com/Doriiaan/Edito/blob/main/doc/Edito.png)

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
