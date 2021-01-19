<!-- https://docs.microsoft.com/en-us/windows/win32/gdi/alpha-blending -->

# Alpha Blending (Windows GDI)

_Alpha blending_ is used to display an alpha bitmap, which is a bitmap that has transparent or semi-transparent pixels. In addition to a red, green, and blue color channel, each pixel in an alpha bitmap has a transparency component known as its _alpha channel_. The alpha channel typically contains as many bits as a color channel. For example, an 8-bit alpha channel can represent 256 levels of transparency, from 0 (the entire bitmap is transparent) to 255 (the entire bitmap is opaque).

Alpha blending mechanisms are invoked by calling `AlphaBlend`, which references the `BLENDFUNCTION` structure.

Alpha values per pixel are only supported for 32-bpp `BI_RGB`. This formala is defined as:

```c
typedef struct {
    BYTE Blue;
    BYTE Green;
    BYTE Red;
    BYTE Alpha;
};
```

This is represented in memory as shown in the following table.

| | | | |
|-|-|-|-|
| 31:24 | 23:16 | 15:08 | 07:00 |
| Alpha | Red | Green | Blue |

Bitmaps may also be displayed with a transparency factor applied to the entire bitmap. Any bitmap format can be displayed with a global constant alpha value by setting `SourceConstantAlpha` in the `BLENDFUNCTION` structure. The global constant alpha value has 256 levels of transparency, from 0 (entire bitmap is completely transparent) to 255 (entire bitmap is completely opaque). The global constant alpha value is combined with the per-pixel value.

For an example, see [Alpha Blending a Bitmap](../using-bitmaps/alpha-blending-a-bitmap.md).

<!-- END -->

[Next - Smooth Shading](./smooth-shading.md)
