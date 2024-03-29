# Purpose
This extension allows displaying thumbnails of OpenEXR (.exr) files.

![screenshot](https://user-images.githubusercontent.com/7930348/69484044-def36900-0e2e-11ea-8a54-206839ba789d.png)

# Compilation
This package requires the following dependencies:
- OpenEXR
- gio-2.0
- gdk-pixbuf-2.0

```
mkdir build
cd build
cmake ..
make install
```
# Packages

## Ubuntu
Currently, there is packages for the following Ubuntu versions
- Ubuntu 20.04 LTS (Focal)
- Ubuntu 18.04 LTS (Bionic)
- Ubuntu 19.04 (Disco)
- Ubuntu 19.10 (Eoan)

To install, simply execute:
```
sudo add-apt-repository ppa:alban-f/openexr-thumbnailer
sudo apt-get update
sudo apt install openexr-thumbnailer
```

## Arch Linux
There is an AUR to ease installation:
```
yay -S openexr-thumbnailer
```
