# interlace

a simple and easy to use picture/photo management application, currently for my own requirements, to work closely with picturecodes Photo Ninja.

To compile the sourcecode, you currently need the following software-packages:  
* QT 4.8.3  
* exiv2 0.23  

The sourcecode is currently developed under linux, but hopefully, on a later stable day, there will be a windows and mac-version, too.

Examples for the configuration-files:

## Windows:
The configuration file should have the name Interlace.cfg and resides in the %USERPROFILE% directory

### Example content:
ImageSize=250
Prg=PhotoNinja;C:\Program Files\PictureCode\PhotoNinja\PhotoNinja64.exe
Prg=Gimp;C:\Program Files\GIMP 2\bin\gimp-2.8.exe
Prg=ImageViewer;C:\Program Files (x86)\FastStone Image Viewer\FSViewer.exe

## Unix
The configuration file should have the name Interlace.cfg and resides in the $HOME directory of the user

### Example content:
ImageSize=250
Prg=PhotoNinja;wine /home/joe/.wine/drive_c/windows/command/start.exe /Unix /home/joe/.wine/drive_c/Programme/PictureCode/PhotoNinja/PhotoNinja32.exe z:\