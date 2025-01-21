# ADDRESS

## Astronomical Database Display and Representation Simple Software

**ADDRESS** is a desktop application able to run on **Windows**, **Linux** and **macOS** systems.\
It is written in **C++** and uses **OpenGL** for graphics.\
With **ADDRESS** you can load and visualize star datasets from **Gaia's** [DR3](https://gea.esac.esa.int/archive/).

<img src="https://user-images.githubusercontent.com/97818552/150414454-452813d0-25c2-46af-ae39-d63be63dd39b.gif" width="768" height="576">

<br>Explore the stars

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/3c3783b2-8d88-4610-b39d-a1960fa10e89.gif" width="768" height="600">

<br>Explore a 3D representation of the catalog

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/daec0f64-c759-4c30-ac9b-d213a8676b32.gif" width="768" height="600">

<br>Explore the catalog in 2D

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/8ca76fae-d693-4feb-a3b5-2e04a19719be.gif" width="768" height="600">

<br>Travel

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/5cdae0a3-9327-4369-83a7-a5f8baef76d9.gif" width="768" height="600">

<br>Visualize the HR Diagram

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/e8b78a3c-0e62-479c-993a-30573ca0c9c3.gif" width="768" height="600">

<br>Visualize the movement of the stars

<img src="https://github.com/juanjo995/ADDRESS/assets/97818552/6f8d2810-3929-4a1a-b691-d4d9e1212961.gif" width="768" height="600">

<br>This project relies on a few libraries to work:

* [**Glad**](https://github.com/Dav1dde/glad) to initialize **OpenGL**.
* [**GLFW**](https://www.glfw.org/) to manage the window creation and events.
* [**ImGui**](https://github.com/ocornut/imgui) to build the user interface.
* [**glm**](https://github.com/g-truc/glm) to deal with different math operations.
* [**CCfits**](https://heasarc.gsfc.nasa.gov/fitsio/CCfits/) and [**cfitsio**](https://heasarc.gsfc.nasa.gov/fitsio/fitsio.html) to load datasets in **FITS** format.
* [**Native File Dialog**](https://github.com/btzy/nativefiledialog-extended) to pop up the open and save file dialog windows.

# Instructions for Linux

## Run on Linux

Just [download](https://github.com/juanjo995/ADDRESS/releases/tag/Linux_Executable) the **ADDRESS.AppImage** file, make sure it has execution permissions (**right click** → **Properties** → **Allow executing file as program**) and double click to execute it, or:

`chmod +x ADDRESS.AppImage`

`./ADDRESS.AppImage`

## Build from source on Linux

We'll need these two libraries in order to run or compile this project.

### Installing cfitsio

First make sure that you have a **C** compiler.

`gcc --version`

If you dont have it you can install it with the next command:

`sudo apt-get install build-essential`

Now we need the **zlib** compression library, we can install it with this command:

`sudo apt-get install zlib1g-dev`

Now download and uncompress the **cfitsio** tar.gz file for Unix, you can download it from here:

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

### Build from source

We must have **OpenGL** installed in our system, we can do it with the next command:

`sudo apt-get install libgl1-mesa-dev`

We also need the RandR headers, we can get these headers with the next command:

`sudo apt-get install xorg-dev`

Native File Dialog library needs GTK, to install it we run the following command:

`sudo apt-get install libgtk-3-dev`

We need to use **cmake**, if you don't have it you can install it with the next command:

`sudo apt-get install cmake`

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

Just [download](https://github.com/juanjo995/ADDRESS/releases/tag/Windows_Executable) the **ADDRESS.zip**, unzip the file and run the executable **ADDRESS.exe**.

## Build from source on Windows

First make sure that you have:

* **Microsoft Visual Studio** available from https://visualstudio.microsoft.com

You must execute the next steps in a **Visual Studio Developer Command Prompt** window as **Administrator**. We recommend the **x64 Native Tools Command Prompt for VS**.

To get this command prompt make sure that you select **Desktop Development with C++** during the installation of Microsoft Visual Studio

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

Now download the **cfitsio** zip file for Windows:

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

### Build from source

Download the repository, enter in the folder and run:

`mkdir build`

`cd build`

Now run cmake.

`cmake ..`

Then we build it.

`cmake --build . --config Release --target ALL_BUILD -j 8 --`

This shall generate the **ADDRESS.exe** file inside the `/build/Release` folder. Copy this file to the path containing de shaders and datasets folder. You will also need to copy to this folder the **.dll** files **zlib.dll** and **cfitsio.dll** that should be inside `C:\Program Files (x86)\zlib\bin`and `C:\Program Files (x86)\CFITSIO\bin`

# Instructions for macOS

## Run on macOS

Just [download](https://github.com/juanjo995/ADDRESS/releases/tag/MacOS_Executable) the **ADDRESS.zip**, unzip the file and run the executable **ADDRESS.app**. The operating system maybe detect that the software is not from a trusted source, in that case you have to go to **System Preferences** → **Security & Privacy** and find the button **Open Anyway**.

## Build from source on macOS

We'll need these two libraries in order to run or compile this project.

First we need the **command line tools for Xcode**, we can install them with the next command:

`xcode-select --install`

### Installing cfitsio

Download and uncompress the **cfitsio** tar.gz file for Unix, you can download it from here:

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

`make`

`sudo make install`

### Build from source

We need to use **cmake**, if you don't have it you can install it with the next command:

`brew install cmake`

If you don't have **brew** you can install it by pasting the command from this page into a terminal:

https://brew.sh/

Now download the repository, enter in the folder and run:

`mkdir build`

`cd build`

`cmake ..`

`make`

This will generate the **ADDRESS** executable file, make sure that you execute it from the same folder that contains the shaders and datasets folders.

`cd ..`

`./build/ADDRESS`

# User manual

In this section, we will explain the basic usage of the software and explore the different sections of the sidebar menu.

## Catalogs, statistics and star names

First, we have a **File** dropdown menu where you can open, save, or close datasets. When saving a dataset, it is saved with the current filter values applied. There is also an option to open the Gaia Archive website.

![1](https://github.com/user-attachments/assets/668ab4a1-957b-42f3-b1d8-a398b47ea8e8)

Next, we have a **Catalog** dropdown menu where you can view the current catalog being displayed and switch to any other catalog you have previously opened.

We also have a **Show stats** button that displays the minimum, maximum, average, variance, and standard deviation of the main parameters in the current catalog.

At the end of this section, we have the option to display star names from the default HYG catalog.

![2](https://github.com/user-attachments/assets/0a375ca1-0252-4232-9865-4867771a5cb6)

## Loading datasets

When loading a dataset, a window will appear allowing us to decide how to handle fields that may contain null values. By default, stars with null values will be discarded, but we can uncheck the checkbox to apply a default value instead.

Additionally, there is a checkbox to determine how to calculate the 3D space velocity. By default, it will only be calculated when both radial velocity and proper motion values are known. Otherwise, the assigned velocity will be 0. However, we can uncheck the checkbox to calculate it using only proper motion or only radial velocity.

![10](https://github.com/user-attachments/assets/ca1cda3e-eef5-4e4a-9097-8823346b51d5)

## Self-adjusting lower distance filter

The purpose of this option is to stabilize the FPS when dealing with large datasets, particularly when the camera is viewing many stars, resulting in a high number of stars on the screen.

![3](https://github.com/user-attachments/assets/ed6508c6-c385-4ef6-9ac3-450fae039188)

## Filters and FOV

You can use the lower and upper sliders to filter by distance, absolute magnitude, color (B-V Index), and velocity, allowing you to customize the rendering dataset based on your criteria.

There is also a button to reset the lower and upper sliders to their minimum and maximum values.

![4](https://github.com/user-attachments/assets/35bf29d5-006a-46b2-bc57-8095c2561a6a)

We also have the option to colorize with the selected filter. This will apply the specified color to the stars based on the three color selectors: the lower color corresponds to the lower filter value, the upper color corresponds to the upper filter value, and the middle color corresponds to the intermediate value between them.

You can increase the influence of the middle color using the middle color factor slider. You can also reset the three color values to the default red, green, and blue. Additionally, you can reset the star colors to their natural color based on the B-V Index.

![5](https://github.com/user-attachments/assets/826be60e-216c-4dc7-b0a0-45586b43af5a)

Next, we have the FOV slider, which allows you to adjust the FOV to any value you desire, along with a button to reset it to its default value of 60. When the camera mode is centered on origin, the FOV can also be adjusted using the mouse wheel.

![6](https://github.com/user-attachments/assets/acb508b3-dbaf-4abe-b137-d1e699530bde)

## Render modes, fundamental plane, and camera modes

In this section, we have the **Render mode** dropdown, which allows us to switch between a 3D space representation of the catalog, a 2D plot, or the HR diagram.

The **Fundamental plane** dropdown allows us to switch between different fundamental planes: Equatorial, Ecliptic, Galactic, Galactocentric, and Horizon View.
The Horizon View represents a fundamental plane based on our position on Earth, which can be specified using the longitude and latitude sliders. There is also a set of buttons to simulate the apparent movement of the stars due to Earth's rotation. Additionally, we have the option to activate different grids corresponding to the selected fundamental planes.

Finally, we have the **Camera mode** dropdown, where we can switch between Centered on origin, Free camera view, and Orbit. In Centered on origin mode, we can change our view direction by clicking and dragging with the left mouse button, and we can zoom in and out using the mouse wheel. In Free camera view, we can move freely through space using the WASD keys, similar to a videogame. The mouse wheel allows us to adjust the velocity at which we are traveling. In Orbit mode, we orbit around the origin of coordinates or around a star. We can change our position by dragging with the left mouse button, and adjust our distance to the orbited point using the mouse wheel.

![7](https://github.com/user-attachments/assets/7b5e0b59-adbd-4bc0-a8ea-54b99a6a2b82)

At any moment, we can press F5 to reset the camera parameters, positioning it at the origin of coordinates and pointing toward the Orion constellation.

## Stars movement

For stars whose speed is known, we have the **Stars movement** option, which provides buttons to advance or rewind time, allowing us to observe the stars' movement over time. There is also an option to render the velocity vectors of these stars, along with a slider to adjust their visibility.

![8](https://github.com/user-attachments/assets/c0e2d0c9-7c5e-42e4-ad30-b660b1e715da)

## Customize visualization

Finally, we can modify the visualization of the stars using three parameters: Size, Alpha, and Halo. For each of these parameters, we have three sliders to precisely adjust the star rendering. In most cases, we only need to use the Offset slider of the Alpha parameter, as it has the greatest effect on the visualization.

![9](https://github.com/user-attachments/assets/2f342aca-4598-4d2b-b417-61b776bab136)

# Downloading datasets from Gaia archive

![gaia](https://github.com/juanjo995/ADDRESS/assets/97818552/8a35cdc7-38a5-4448-9dfd-ec4bbd7ec368)

We can access to the Gaia archive through the next link:

https://gea.esac.esa.int/archive/

![interface](https://github.com/juanjo995/ADDRESS/assets/97818552/a1b63b12-3fa2-4f07-b2b7-fa10c2f0bafe)

In the **Search in** dropdown we must select **gaiadr3.gaia_source**.

In the section **Extra conditions** we can add as many conditions as we want.

In the section **Display columns** we must select at least the next atributes:
* ra
* dec
* parallax
* pmra
* pmdec
* phot_g_mean_mag
* bp_g
* radial_velocity

The section **Advanced (ADQL)** shows us the specific query that was generated, we can modify it or make some changes like ordering by random index which will make the generated dataset statitically representative of the entire catalog.

![query](https://github.com/juanjo995/ADDRESS/assets/97818552/26dc8967-40c4-43f1-84c5-90344469582e)

Once the query is ready we can click on **Submit Query**, wait for the output and download the result in **CSV** (plain text) or **FITS** (binary) format.

We recommend downloading the catalogs in **FITS** format, as being a binary format, the file takes up significantly less space than a **CSV**.

If you use this site without being registered the maximum number of stars in the output dataset is limited to 3 million. You will need to create an account and log in to download larger datasets.

By default **ADDRESS** comes with one dataset, the [HYG](https://github.com/astronexus/HYG-Database) database which contains the brightest stars and the common name of some of them.

## Some example queries

```sql
-- 10 million stars where absolute magnitude, color index and parallax are not null and parallax is positive.
SELECT TOP 10000000 gaia_source.ra,gaia_source.dec,gaia_source.parallax,gaia_source.pmra,gaia_source.pmdec,gaia_source.phot_g_mean_mag,gaia_source.bp_g,gaia_source.radial_velocity
FROM gaiadr3.gaia_source
WHERE gaia_source.parallax IS NOT NULL AND gaia_source.phot_g_mean_mag IS NOT NULL AND gaia_source.bp_g IS NOT NULL AND (gaiadr3.gaia_source.parallax>0.0)
ORDER BY gaia_source.random_index
```

```sql
-- 500 thousand stars where the velocity parameters are known, so we can calculate the 3D space velocity.
SELECT TOP 500000 gaia_source.ra,gaia_source.dec,gaia_source.parallax,gaia_source.pmra,gaia_source.pmdec,gaia_source.phot_g_mean_mag,gaia_source.bp_g,gaia_source.radial_velocity
FROM gaiadr3.gaia_source
WHERE gaia_source.parallax IS NOT NULL AND gaia_source.phot_g_mean_mag IS NOT NULL AND gaia_source.bp_g IS NOT NULL AND (gaiadr3.gaia_source.parallax>0.0) AND gaia_source.pmra IS NOT NULL AND gaia_source.pmdec IS NOT NULL AND gaia_source.radial_velocity IS NOT NULL
ORDER BY gaia_source.random_index
```
