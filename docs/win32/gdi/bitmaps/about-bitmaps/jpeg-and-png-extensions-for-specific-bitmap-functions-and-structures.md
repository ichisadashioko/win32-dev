<!-- https://docs.microsoft.com/en-us/windows/win32/gdi/jpeg-and-png-extensions-for-specific-bitmap-functions-and-structures -->

On certain versions of Microsoft Windows, the [`StretchDIBits`][nf-wingdi-stretchdibits] and [`SetDIBitsToDevice`][nf-wingdi-setdibitstodevice] functions allow JPEG and PNG images to be passed as the source image to printer devices. This extension is not intended as a means to supply general JPEG and PNG decompression to applications, but rather to allow applications to send JPEG- and PNG-compressed images directly to printers having hardware support for JPEG and PNG images.

The `BITMAPINFOHEADER`, `BITMAPV4HEADER` and `BITMAPV5HEADER` structures are extended to allow specification of `biCompression` values indicating that the bitmap data is a JPEG or PNG image. These compression values are only valid for `SetDIBitsToDevice` and `StretchDIBits` when the `hdc` parameter specifies a printer device. To support metafile spooling of the printer, the application should not rely on the return value to determine whether the device supports the JPEG or PNG file. The application must issue `QUERYESCSUPPORT` with the corresponding escape before calling `SetDIBitsToDevice` and `StretchDIBits`. If the validation escape fails, the application must then fall back on its own JPEG or PNG support to decompress the image into a bitmap.

[nf-wingdi-stretchdibits]: https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-stretchdibits
[nf-wingdi-setdibitstodevice]: https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-setdibitstodevice

<!-- END -->

[Next - Bitmaps, Device Contexts, and Drawing Surfaces](./bitmaps-device-contexts-and-drawing-surfaces.md)

__shioko's note__

I think this guide clear up a little bit about "bitmap" for me.
