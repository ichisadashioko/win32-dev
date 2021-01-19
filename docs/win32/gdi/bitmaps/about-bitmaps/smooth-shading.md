<!-- https://docs.microsoft.com/en-us/windows/win32/gdi/smooth-shading -->

# Smooth Shading

_Smooth shading_ is a method of shading a region with a color gradient. Including color information, along with the bounds of drawing primitive, specifies the color gradient. GDI linearly interpolates the color of the inside of the primitive passed on the color endpoints. Color and vertex information is included with position information in the `TRIVERTEX` structure.

Use the `GradientFill` function to fill a triangle or rectangle structure. To fill a triangle with smoothing shading, call `GradientFill` with three triangle endpoints. To fill a rectangle with smooth shading, call `GradientFill` with the upper-left and lower-right rectangle coordinates. `GradientFill` references the `TRIVERTEX`, `GRADIENT_RECT`, and `GRADIENT_TRIANGLE` structures.

For an example, see [Drawing a Shaded Triangle](../using-bitmaps/drawing-a-shaded-triangle.md)

<!-- END -->

[Next - ICM-Enabled Bitmap Functions](./icm-enabled-bitmap-functions.md)
