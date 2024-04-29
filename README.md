# ADDRESS

## Astronomical Database Display and Representation Simple Software

**ADDRESS** is a desktop application able to run on both **Windows** and **Linux** systems.\
It is written in **C++** and uses **OpenGL** for graphics.\
With **ADDRESS** you can load and visualize stars datasets from **Gaia's** [GEDR3](https://gea.esac.esa.int/archive/)

<img src="https://user-images.githubusercontent.com/97818552/150414454-452813d0-25c2-46af-ae39-d63be63dd39b.gif" width="768" height="576">

Explore the stars

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/ba4f19cb-bb70-4f85-a6b6-c22a2cb0015e.gif" width="512" height="384">

Colorize the catalog

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/69fef033-04b2-43e0-8076-706798379ae5.gif" width="512" height="384">

Explore a 3D representation of the catalog

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/e9e3faa6-ff65-4ca8-afbd-cb82345e679d.gif" width="512" height="384">

Explore the catalog in 2D

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/cfc0e9a0-0946-48a0-9151-2a94054a880d.gif" width="512" height="384">

Travel

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/86dd1a6c-a88d-44fa-b885-12f31fde1c6e.gif" width="512" height="384">

Visualize the HR Diagram

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/042252d0-60f2-4fed-ae55-c493364729e3.gif" width="512" height="384">

Visualize the movement of the stars

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/496e90de-9e78-4d32-ba4e-d1e643c8a9d8.gif" width="512" height="384">

<br>This project relies on a few libraries to work

**Glad** to initialize **OpenGL**\
**GLFW** to manage the window creation and events\
**ImGui** to build the user interface\
**glm** to deal with different math operations\
**CCfits** and **cfitsio** to load datasets in **FITS** format

## Installing cfitsio and CCfits

We'll need these two libraries in order to run or compile this project

### Installing cfitsio

First make sure that you have a **C** compiler

`gcc --version`

If you dont have it you can install it with the next command

`sudo apt-get install build-essential`

Now we need the **zlib** compression library, we can install it with this command

`sudo apt-get install zlib1g-dev`

Now download and uncompress the **cfitsio** tar.gz file, you can download it from here

https://heasarc.gsfc.nasa.gov/fitsio/fitsio.html

Enter in the folder and execute the next commands

`./configure --prefix=/usr/local`

`make`

`sudo make install`

### Installing CCfits

Now download and uncompress the **CCfits** tar.gz file, you can download it from here

https://heasarc.gsfc.nasa.gov/fitsio/CCfits/

Enter in the folder and execute the next commands

`./configure --prefix=/usr/local --with-cfitsio-include=/usr/local/include --with-cfitsio-libdir=/usr/local/lib`

`gmake`

`sudo make install`

### Update your libraries

Now we need to update these new libraries with the command `updatedb`, if you don't have it you can install it with the next command

`sudo apt-get install mlocate`

We now run the `updatedb` command to update these new libraries

`sudo updatedb`

## Run on Linux

Once you installed **cfitsio** and **CCfits** you can just [download](https://github.com/juanjo995/ADDRESS/releases/tag/Executable) and run the executable

Make sure it can be executed

`chmod +x ADDRESS`

Run it

`./ADDRESS`

## Build from source on Linux

We must have **OpenGL** installed in our system, we can do it with the next command

`sudo apt-get install libgl1-mesa-dev`

We also need the RandR headers, we can get these headers with the next command

`sudo apt-get install xorg-dev`

We need to use **cmake**, if you don't have it you can install it with the next command

`supo apt-get install cmake`

Now download the repository, enter in the folder and run

`mkdir build`

`cd build`

`cmake ..`

`make`

This will generate the **ADDRESS** executable file, make sure that you execute it from the same folder that contains the shaders and datasets folders

`cd ..`

`./build/ADDRESS`
