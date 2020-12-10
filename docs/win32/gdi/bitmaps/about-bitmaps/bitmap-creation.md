<!-- https://docs.microsoft.com/en-us/windows/win32/gdi/bitmap-creation -->

# Bitmap Creation

To create a bitmap, use the `CreateBitmap`, `CreateBitmapIndirect`, or `CreateCompatibleBitmap` function, `CreateDIBitmap`, and `CreatetDiscardableBitmap`.

These functions allow you to specify the width and height, in pixels, of the bitmap. The `CreateBitmap` and `CreateBitmapIndirect` function also allow you to specify the number of color planes and the number of bits required to identify the color. On the other hand, the `CreateCompatibleBitmap` and `CreateDiscardableBitmap` functions use a specified device context to obtain the number of color planes and the number of bits required to identify the color.

The `CreateDIBitmap` function creates a device-dependent bitmap from a device-independent bitmap. It contains a color table that describes how pixel values correspond to RGB color values. For more information, see [Device-Dependent Bitmaps](./bitmaps-classifications/device-dependent-bitmaps.md) and [Device-Independent Bitmaps](./bitmaps-classifications/device-independent-bitmaps.md)

After the bitmap has been created, you cannot changes its size, number of color planes, or number of bits required to identify the color.

When you no longer need a bitmap, call the `DeleteObject` function to delete it.

<!-- END -->

[Next - Bitmap Rotation](./bitmap-rotation.md)

__shioko's note__

Device-dependent bitmap and device-independent bitmap still confuse me. It is probably because they were introduced pretty early while I was still about this "bitmap" thing is what I need instead of multi-dimension array for storing image data in memory (coming from Python and OpenCV).
