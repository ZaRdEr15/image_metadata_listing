# JPEG Image metadata listing in C++ test work for Artec Design Internship.

The console utility prints on the screen in the style of ```ls```:

* Name of the file, 
* Capture date,
* Camera model 

Based on the EXIF metadata from the JPEG image.

## Built with

**Make** is used to build the C++17 utility for *Linux* based systems.

[TinyEXIF](https://github.com/cdcseacave/TinyEXIF) was used to parse EXIF metadata from the JPEG files.
The **Samples** folder contains different JPEG images for testing taken from the [same repository](https://github.com/cdcseacave/TinyEXIF). TinyEXIF is dependent on tinyxml2 to be able to build, so both the TinyEXIF and tinyxml2 are located in their respective folders in **src**.
[Google test/gtest](https://github.com/google/googletest) is used for testing and it's makefile and testing code is located in the **test** folder.

## Installation

To install from the repository, clone the repository to your own machine (you must have access because the repository is private):
```https://github.com/ZaRdEr15/image_metadata_listing.git```

To use **gtest**, either install it:

```shell
sudo apt update
sudo apt install libgtest-dev
```

Or run the ```./install.sh``` bash script file to execute exactly the same commands.

## How to build

To build the utility run ```./build.sh``` script from the root of the project. This will build the utility in **build** using *make* and add a linker to the executable to root called **img_metadata**. The script also creates a **build** folder if it was not present before.

To build the tests navigate to the **test** folder and run ```make``` to build the individual tests *arguments_unittest* and *parser_unittest*. To remove test build components run ```make clean```.

Run the tests using ```./arguments_unittest``` and ```./parser_unittest``` respectively.

You might need to tweak this line inside **test/Makefile** -> ```GTEST_DIR = /usr/src/gtest``` to fit your specific system in order for the tests to build.

## Usage

To use the utility, run ```./img_metadata``` (if ran from the root folder of project, otherwise [location_to_project/img_metadata]) in the terminal from the root folder to see the usage:
```Usage: ./img_metadata [-n "file_name"] [-d "capture_date"] [-m "camera_model"] directory```

Every option that is inside the brackets is optional, **directory** is required.

*File name* and *camera model* are case insensitive options and support '*' wildcard symbol for partial matching, such as:
```Patterns that match "abcde" <- "*de", "ab*", "*c*", "ab*e", "****e", "***b***d**", etc.```

*Capture date* only supports exact match, such as ```"2018-05-20" <- "2018-05-20"```

Every optional option **must be inside quatation marks** (otherwise the asterisk wildcard symbol would not be passed to the utility)!

You **don't** have to provide '.jpg' extension to the file name, it happens automatically.

Either the EXIF metadata might not have any available fields or the capture date and the camera model might not exist in those fields.

Example usage and output (ran from the root of project):

### Show all files inside folder recursively
```shell
./img_metadata Samples/

"Anafi.jpg"                                       Capture Date: 2018-05-20      Camera Model: ANAFI                         
"test3.jpg"                                       Capture Date: 2012-12-22      Camera Model: iPhone 4S                     
"evil1.jpg"                                       Capture Date: 2003-05-24      Camera Model: Canon PowerShot S400          
"003431ad6fd3b86a5493105ef3c8db49.jpg"            Capture Date: 2018-09-20      Camera Model: XT2                           
"example005.jpg"                                  Capture Date: Doesn't exist   Camera Model: Doesn't exist                 
"lukas12p.jpg"                                    Capture Date: 2008-09-02      Camera Model: E-510                         
"test1.jpg"                                       Capture Date: 2013-02-06      Camera Model: iPhone 4S                     
"1103806289718.jpg"                               Capture Date: 2017-01-17      Camera Model: Canon PowerShot SX260 HS
...
X JPEG files found
```
### Show all files starting with "test"

```shell
./img_metadata -n "test*" Samples/

"test3.jpg"                                       Capture Date: 2012-12-22      Camera Model: iPhone 4S                     
"test1.jpg"                                       Capture Date: 2013-02-06      Camera Model: iPhone 4S                     
"test2.jpg"                                       Capture Date: 2012-12-23      Camera Model: iPhone 4S                     
3 JPEG files found
```

### Show all files ending with a number 2

```shell
./img_metadata -n "*2" Samples/

"VRAexample012.jpg"                               Capture Date: Doesn't exist   Camera Model: Doesn't exist                 
"Bebop_2.jpg"                                     Capture Date: 2016-11-04      Camera Model: Bebop 2                       
"test2.jpg"                                       Capture Date: 2012-12-23      Camera Model: iPhone 4S                     
3 JPEG files found
```

### Show all files that used a camera with "*PowerShot*" in the name

```shell
./img_metadata -m "*powershot*" Samples/

"evil1.jpg"                                       Capture Date: 2003-05-24      Camera Model: Canon PowerShot S400          
"1103806289718.jpg"                               Capture Date: 2017-01-17      Camera Model: Canon PowerShot SX260 HS      
2 JPEG files found
```

### Show all files that have a name "sensefly", capture date "2017-09-20" and a camera model "S.O.D.A."

```shell
./img_metadata -n "sensefly" -d "2017-09-20" -m "s.o.d.a." Samples/

"sensefly.jpg"                                    Capture Date: 2017-09-20      Camera Model: S.O.D.A.                      
1 JPEG files found
```


