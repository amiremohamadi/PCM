# PCM (producer consumer module)
linux kernel driver for **producer-consumer** problem.

## Installation
### Requirements
- LINUX-Based operating system
- C-Compiler ([GCC](https://github.com/gcc-mirror/gcc) is preferred)
- Kernel-Headers

#### Install kernel headers
If you are using a debian-based distro (such as ubuntu) try this:

`sudo apt install linux-headers-$(uname -r)` 

For fedora users:

`sudo dnf install kernel-devel kernel-headers-$(uname -r)`

### Install
```
git clone https://github.com/amiremohamadi/PCM
cd PCM
make
```

## Usage
`sudo insmod pcm.ko`

And to see the result:

`dmesg --follow`

## Maintainer
[@amiremohamadi](https://github.com/amiremohamadi)

## License
Under GPL v2 license
