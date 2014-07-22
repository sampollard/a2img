a2img

The idea here is to convert any file in to an image.

To build:
Just type make.
make clean deletes the .o and executable files

To use:
a2img [options] file-to-convert-to-image image-name

If you just give one argument a2img will create a file with the same name but
with the extension of the image type appended to it. If an output filename is
provided, the file extension will not be appended. For example,

a2img myfile.txt

will create a file called
myfile.txt.ppm

The extension by default is plain ppm. The supported output filetypes are:
plain ppm
raw ppm

options:
These are flags that start with a "-". Please don't start the filename with "-"
because a2img will think your filename is a flag. Here are the list of flags:

-v : verbose; explains what is happening
-h : help; opens this file.
-r : raw-ppm; applies only if a ppm file is used. Prints the raster as an
	 array of pure binary instead of ASCII decimal numbers. If this option
	 is checked the file looks exactly like the original but with a header
	 that starts with "P6 width height 255". It is also about 1/3 the size of
	 a plain ppm.
-o : offset; this flag must be immediately followed by the number of BITS that
	 you want to offset, to a maximum of INT_MAX.
	 example: a2img -o 3 pic.bmp /newloc/pic.ppm