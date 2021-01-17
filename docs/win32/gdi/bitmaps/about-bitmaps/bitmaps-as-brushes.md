<!-- https://docs.microsoft.com/en-us/windows/win32/gdi/bitmaps-as-brushes -->

# Bitmaps as Brushes

A number of functions use the brush currently selected into a device context to perform bitmap operations. For example, the `PatBlt` function replicates the brush in a rectangular region within a window, and the `FloodFill` function replicates the brush inside an area in a window bounded by the specified color (unlike `PatBlt`, `FloodFill` does fill nonrectangular shapes).

The `FloodFill` function replicates the brush within a region bounded by a specified color. However, unlike the `PatBlt` function, `FloodFill` does not combine the color data for the brush with the color data for the pixels on the display; it simply sets the color of all pixels within the enclosed region on the display to the color of the brush that is currently selected into the device context.

<!-- END -->

[Next - Bitmap Storage](./bitmap-storage.md)

__shioko's note__

I feel like I am learning to create MS Paint clone - the `FloodFill` function.
