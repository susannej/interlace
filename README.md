# interlace

a simple and easy to use picture/photo management application, currently for my own requirements, to work closely with picturecodes Photo Ninja.

To compile the sourcecode, you currently need the following software-packages:  
* QT 5.11
* exiv2 0.27-RC4  

The sourcecode is currently developed under linux, but hopefully, on a later stable day, there will be a windows and mac-version, too.

To create the system-dependend makefiles, please type:  
"qmake" on unix-based systems and "qmake -t vcapp" for Windows Visual C++ based systems  

Examples for the configuration-files:

## Windows:
The configuration file should have the name Interlace.cfg and resides in the %USERPROFILE% directory

### Example content:
ImageSize=250  
StartDirectory=C:\Benutzer\joe\Eigene Bilder  
Prg=PhotoNinja;"C:\Program Files\PictureCode\PhotoNinja\PhotoNinja64.exe" "${file}"  
Prg=Gimp;"C:\Program Files\GIMP 2\bin\gimp-2.8.exe" "${file}"  
Prg=ImageViewer;"C:\Program Files (x86)\FastStone Image Viewer\FSViewer.exe" "${file}"  
Label=Red;Red  
Label=Yellow;Yellow  
Label=Blue;Blue  
Label=Green;Green  
Label=Purple;Purple  
ExifKeys=Exif.Image.ImageDescription;Exif.Image.ImageWidth;Exif.Image.ImageLength;Exif.Image.Make;Exif.Image.Model;Exif.Image.DateTime;Exif.Image.Artist;Exif.Image.Copyright;Exif.Image.Software;Exif.Photo.ExposureTime;Exif.Photo.FNumber;Exif.Photo.ExposureProgram;Exif.Photo.ISOSpeedRatings;Exif.Photo.DateTimeOriginal;Exif.Photo.ExposureBiasValue;Exif.Photo.MaxApertureValue;Exif.Photo.MeteringMode;Exif.Photo.LightSource;Exif.Photo.Flash;Exif.Photo.FocalLength;Exif.Photo.UserComment;Exif.Photo.ColorSpace;Exif.Photo.FileSource;Exif.Photo.ExposureMode;Exif.Photo.WhiteBalance;Exif.Photo.DigitalZoomRatio;Exif.Photo.Contrast;Exif.Photo.Saturation;Exif.Photo.Sharpness;Exif.Photo.LensSpecification;Exif.Photo.LensModel;Xmp.dc.creator;Xmp.dc.subject;Xmp.lr.hierarchicalSubject  

## Unix
The configuration file should have the name Interlace.cfg and resides in the $HOME directory of the user

### Example content:
ImageSize=250  
StartDirectory=/home/joe/Bilder  
Prg=PhotoNinja;wine /home/joe/.wine/drive_c/windows/command/start.exe /Unix /home/joe/.wine/drive_c/Programme/PictureCode/PhotoNinja/PhotoNinja32.exe "z:/${file}"  
Prg=Gimp;gimp "${file}"  
Label=Red;Red  
Label=Yellow;Yellow  
Label=Blue;Blue  
Label=Green;Green  
Label=Purple;Purple  
ExifKeys=Exif.Image.ImageDescription;Exif.Image.ImageWidth;Exif.Image.ImageLength;Exif.Image.Make;Exif.Image.Model;Exif.Image.DateTime;Exif.Image.Artist;Exif.Image.Copyright;Exif.Image.Software;Exif.Photo.ExposureTime;Exif.Photo.FNumber;Exif.Photo.ExposureProgram;Exif.Photo.ISOSpeedRatings;Exif.Photo.DateTimeOriginal;Exif.Photo.ExposureBiasValue;Exif.Photo.MaxApertureValue;Exif.Photo.MeteringMode;Exif.Photo.LightSource;Exif.Photo.Flash;Exif.Photo.FocalLength;Exif.Photo.UserComment;Exif.Photo.ColorSpace;Exif.Photo.FileSource;Exif.Photo.ExposureMode;Exif.Photo.WhiteBalance;Exif.Photo.DigitalZoomRatio;Exif.Photo.Contrast;Exif.Photo.Saturation;Exif.Photo.Sharpness;Exif.Photo.LensSpecification;Exif.Photo.LensModel;Xmp.dc.creator;Xmp.dc.subject;Xmp.lr.hierarchicalSubject  
