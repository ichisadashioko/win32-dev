<!-- https://docs.microsoft.com/en-us/windows/win32/gdi/icm-enabled-bitmap-functions -->

# ICM-Enabled Bitmap Functions

Microsoft Image Color Management (ICM) ensures that a color image, graphic object, or text object is rendered as closely as possible to its original intent on any device, despite differences in imaging technologies and color capabilities between devices. Whether you are scanning an image or other graphic on a color scanner, downloading it over the Internet, viewing or editing it onscreen, or printing it on paper, film, or other media, ICM 2.0 helps you keep colors consistent and accurate. For more information about ICM, see [Windows Color System](https://docs.microsoft.com/en-us/previous-versions/dd372446(v=vs.85)).

There are various functions in the graphics device interface (GDI) that use or operate on color data. The following bitmap functions are enabled for use with ICM:

- `BitBlt`
- `CreateDIBitmap`
- `CreateDIBSection`
- `MaskBlt`
- `SetDIBColorTable`
- `StretchBlt`
- `SetDIBits`
- `SetDIBitsToDevice`
- `StretchDIBits`

<!-- END -->

[Next - Using Bitmaps](../using-bitmaps/readme.md)
