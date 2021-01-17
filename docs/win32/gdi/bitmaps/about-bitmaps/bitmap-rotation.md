<!-- https://docs.microsoft.com/en-us/windows/win32/gdi/bitmap-rotation -->

# Bitmap Rotation
To copy a bitmap into a [parallelogram](https://en.wikipedia.org/wiki/Parallelogram); use the `PlgBlt` function, which performs a bit-block transfer from a rectangle in a source device context into a parallelogram in a destination device context. To rotate the bitmap, an application must provide the coordinates, in world units, to be used for the corners of the parallelogram. (For more information about rotation and world units, see [Coordinate Spaces and Transformations](../../coordinate-spaces-and-transformations/readme.md).)

<!-- END -->

[Next - Bitmap Scaling](./bitmap-scaling.md)
