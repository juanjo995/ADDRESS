# ADDRESS

## Astronomical Database Display and Representation Simple Software

**ADDRESS** is a desktop application able to run on both **Windows** and **Linux** systems.\
It is written in **C++** and uses **OpenGL** for graphics.\
With **ADDRESS** you can load and visualize stars datasets from **Gaia's** [GEDR3](https://gea.esac.esa.int/archive/).

<img src="https://user-images.githubusercontent.com/97818552/150414454-452813d0-25c2-46af-ae39-d63be63dd39b.gif" width="768" height="576">

<br>Explore the stars

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/ba4f19cb-bb70-4f85-a6b6-c22a2cb0015e.gif" width="512" height="384">

<br>Colorize the catalog

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/69fef033-04b2-43e0-8076-706798379ae5.gif" width="512" height="384">

<br>Explore a 3D representation of the catalog

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/e9e3faa6-ff65-4ca8-afbd-cb82345e679d.gif" width="512" height="384">

<br>Explore the catalog in 2D

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/cfc0e9a0-0946-48a0-9151-2a94054a880d.gif" width="512" height="384">

<br>Travel

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/86dd1a6c-a88d-44fa-b885-12f31fde1c6e.gif" width="512" height="384">

<br>Visualize the HR Diagram

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/042252d0-60f2-4fed-ae55-c493364729e3.gif" width="512" height="384">

<br>Visualize the movement of the stars

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/496e90de-9e78-4d32-ba4e-d1e643c8a9d8.gif" width="512" height="384">

<br>This project relies on a few libraries to work:

* **Glad** to initialize **OpenGL**
* **GLFW** to manage the window creation and events
* **ImGui** to build the user interface
* **glm** to deal with different math operations
* **CCfits** and **cfitsio** to load datasets in **FITS** format

# Instructions for Linux

We'll need these two libraries in order to run or compile this project.

### Installing cfitsio

First make sure that you have a **C** compiler.

`gcc --version`

If you dont have it you can install it with the next command:

`sudo apt-get install build-essential`

Now we need the **zlib** compression library, we can install it with this command:

`sudo apt-get install zlib1g-dev`

Now download and uncompress the **cfitsio** tar.gz file, you can download it from here:

https://heasarc.gsfc.nasa.gov/fitsio/fitsio.html

Enter in the folder and execute the next commands:

`./configure --prefix=/usr/local`

`make`

`sudo make install`

### Installing CCfits

Now download and uncompress the **CCfits** tar.gz file, you can download it from here:

https://heasarc.gsfc.nasa.gov/fitsio/CCfits/

Enter in the folder and execute the next commands:

`./configure --prefix=/usr/local --with-cfitsio-include=/usr/local/include --with-cfitsio-libdir=/usr/local/lib`

`gmake`

`sudo make install`

### Update your libraries

Now we need to update these new libraries with the command `updatedb`, if you don't have it you can install it with the next command:

`sudo apt-get install mlocate`

We now run the `updatedb` command to update these new libraries.

`sudo updatedb`

## Run on Linux

Once you installed **cfitsio** and **CCfits** you can just [download](https://github.com/juanjo995/ADDRESS/releases/tag/Executable) the **ADDRESS.zip**, unzip the file and run the executable **ADDRESS**.

Make sure it can be executed.

`chmod +x ADDRESS`

Run it.

`./ADDRESS`

## Build from source on Linux

We must have **OpenGL** installed in our system, we can do it with the next command:

`sudo apt-get install libgl1-mesa-dev`

We also need the RandR headers, we can get these headers with the next command:

`sudo apt-get install xorg-dev`

We need to use **cmake**, if you don't have it you can install it with the next command:

`supo apt-get install cmake`

Now download the repository, enter in the folder and run:

`mkdir build`

`cd build`

`cmake ..`

`make`

This will generate the **ADDRESS** executable file, make sure that you execute it from the same folder that contains the shaders and datasets folders.

`cd ..`

`./build/ADDRESS`

# Instructions for Windows

## Run on Windows

Just [download](https://github.com/juanjo995/ADDRESS/releases/tag/Executable) the **ADDRESS.zip**, unzip the file and run the executable **ADDRESS.exe**.

## Build from source on Windows

### ccfits and cfitsio

First make sure that you have:

* Microsoft Visual Studio
* The CMake build system available from http://www.cmake.org

Execute the next steps in a **Visual Studio Developer Command Prompt** window as **Administrator**. We recommend the **x64 Native Tools Command Prompt for VS**.

### Installing zlib

First we need to download the **zlib** library, we can download it from here:

https://zlib.net

Extract the tar.gz file and enter in the folder.

`mkdir build`

`cd build`

`cmake ..`

Build.

`cmake --build . --config Release`

And install it.

`cmake --install .`

This shall install the library into `C:\Program Files (x86)\zlib`

### Installing cfitsio

Now download the **cfitsio** file for Windows:

http://heasarc.gsfc.nasa.gov/fitsio

Extract the zip file and enter in the folder.

`mkdir build`

`cd build`

Run the next command specifying your path to zlib that should be `C:\Program Files (x86)\zlib`

`cmake .. -DCMAKE_PREFIX_PATH="C:\Program Files (x86)\zlib"`

Now run the next command:

`cmake --build . --config Release`

And install it.

`cmake --install .`

This shall install the library into `C:\Program Files (x86)\CFITSIO`

### Installing CCfits

Download the CCfits source code from here:

https://heasarc.gsfc.nasa.gov/fitsio/CCfits/

Extract the file and enter in the folder.

`mkdir build`

`cd build`

Run the next command specifying the path to **cfitsio** that should be `C:\Program Files (x86)\CFITSIO`

`cmake -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:\Program Files (x86)\CFITSIO" ..`

`nmake`

`nmake install`

This shall install the library into `C:\Program Files (x86)\CCfits`

### Building from source

Download the repository, enter in the folder and run:

`mkdir build`

`cd build`

Now run cmake.

`cmake ..`

Then we build it.

`cmake --build . --config Release --target ALL_BUILD -j 8 --`

This shall generate the **ADDRESS.exe** file inside the `/build/Release` folder. Copy this file to the path containing de shaders and datasets folder. You will also need to copy to this folder the **.dll** files **zlib.dll** and **cfitsio.dll** that should be inside `C:\Program Files (x86)\zlib\bin`and `C:\Program Files (x86)\CFITSIO\bin`

## Downloading datasets from Gaia archive

![gaia](https://github.com/juanjo995/ADDRESS/assets/97818552/8a35cdc7-38a5-4448-9dfd-ec4bbd7ec368)

We can access to the Gaia archive through the next link:

https://gea.esac.esa.int/archive/

![interface](https://github.com/juanjo995/ADDRESS/assets/97818552/c63da13e-1eaa-496e-817e-92715f8a0e20)

In the **Search in** dropdown we must select **gaiaedr3.gaia_source** (do not confuse with gaiadr3.gaia_source).

In the section **Extra conditions** we can add as many conditions as we want.

In the section **Display columns** we must select at least the next atributes:
* ra
* dec
* parallax
* pmra
* pmdec
* phot_g_mean_mag
* bp_g
* dr2_radial_velocity

The section **Advanced (ADQL)** shows us the specific query that was generated, we can modify it or make some changes like ordering by random index which will make the generated dataset statitically representative of the entire catalog.

![query](https://github.com/juanjo995/ADDRESS/assets/97818552/80ac5dd2-c425-4132-ab31-6ad109b63cb8)

Once the query is ready we can click on **Submit Query**, wait for the output and download the result in **CSV** (plain text) or **FITS** (binary) format.

If you use this site without being registered the maximum number of stars in the output dataset is limited to 3 million, you'll need to create an account and log in to download larger datasets.
