a2img

The idea here is to convert any file in to an image.

To use:
a2img [options] file-to-convert-to-image image-name

If you just give one argument a2img will create a file with the same name but
with the extension of the image type appended to it. For example,

a2img myfile.txt

will create a file called
myfile.txt.ppm

The extension by default is ppm. The supported filetypes are:
plain ppm

options:
These are flags that start with a "-". Please don't start the filename as "-"
because a2img will think your filename is a flag. Here are the list of flags:

-v : verbose; tells what's happening
-h : help; opens this file.
